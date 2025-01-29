#pragma once

#include <iostream>
#include <unordered_map>
#include <string>
#include <string_view>
#include <functional>
#include <stdexcept>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <mutex>
#include <queue>
#include <vector>
#include <optional>
#include <framework/transport/TransportCallbacks.h>
#include <framework/socket/tcp/SocketException.h>

namespace hyper::framework
{
    class EpollSocketManager
    {
    public:
        EpollSocketManager()
        {
            epoll_fd_ = epoll_create1(0);
            if (epoll_fd_ == -1)
            {
                throw SocketException("Failed to create epoll instance");
            }
        }

        ~EpollSocketManager()
        {
            for (auto &[fd, _] : connections_)
            {
                close(fd);
            }
            close(epoll_fd_);
        }

        int add_server(const std::string &ip, int port, const SocketCallbacks &callbacks)
        {
            int server_fd = create_server_socket(ip, port);
            register_socket_to_epoll(server_fd, EPOLLIN | EPOLLET | EPOLLOUT, callbacks, true);
            return server_fd;
        }

        int add_client(const std::string &ip, int port, const SocketCallbacks &callbacks)
        {
            int client_fd = create_client_socket(ip, port);
            register_socket_to_epoll(client_fd, EPOLLIN | EPOLLET | EPOLLOUT, callbacks, false);

            // Immediately notify about the connection if callback is set
            if (callbacks.connect_callback_)
            {
                callbacks.connect_callback_();
            }

            return client_fd;
        }

        void process_io_events()
        {
            constexpr int max_events = 16;
            epoll_event events[max_events];

            while (true)
            {
                int event_count = epoll_wait(epoll_fd_, events, max_events, -1);
                if (event_count == -1)
                {
                    if (errno == EINTR)
                        continue;
                    throw SocketException("epoll_wait failed");
                }

                for (int i = 0; i < event_count; ++i)
                {
                    handle_io_event(events[i]);
                }
            }
        }

        void send_data(int fd, std::string_view data)
        {
            std::lock_guard<std::mutex> lock(send_mutex_);
            ssize_t bytes_sent = send(fd, data.data(), data.size(), 0);
            if (bytes_sent == -1 && errno != EAGAIN)
            {
                std::cerr << "Failed to send data on fd: " << fd << "\n";
            }
        }

        void run()
        {
            process_io_events();
        }

    private:
        struct Connection
        {
            int socket_fd_;
            bool is_server_;
            SocketCallbacks callbacks_;
            char buffer_[4096]{};
        };

        int epoll_fd_;
        std::unordered_map<int, Connection> connections_;
        std::mutex send_mutex_;

        int create_server_socket(const std::string &ip, int port)
        {
            int server_fd = socket(AF_INET, SOCK_STREAM, 0);
            if (server_fd == -1)
            {
                throw SocketException("Failed to create server socket");
            }

            set_socket_non_blocking(server_fd);

            sockaddr_in server_addr{};
            server_addr.sin_family = AF_INET;
            server_addr.sin_port = htons(port);
            if (inet_pton(AF_INET, ip.c_str(), &server_addr.sin_addr) <= 0)
            {
                close(server_fd);
                throw SocketException("Invalid IP address");
            }

            if (bind(server_fd, reinterpret_cast<sockaddr *>(&server_addr), sizeof(server_addr)) == -1)
            {
                close(server_fd);
                throw SocketException("Failed to bind server socket");
            }

            if (listen(server_fd, 10) == -1)
            {
                close(server_fd);
                throw SocketException("Failed to listen on server socket");
            }

            return server_fd;
        }

        int create_client_socket(const std::string &ip, int port)
        {
            int client_fd = socket(AF_INET, SOCK_STREAM, 0);
            if (client_fd == -1)
            {
                throw SocketException("Failed to create client socket");
            }

            set_socket_non_blocking(client_fd);

            sockaddr_in server_addr{};
            server_addr.sin_family = AF_INET;
            server_addr.sin_port = htons(port);
            if (inet_pton(AF_INET, ip.c_str(), &server_addr.sin_addr) <= 0)
            {
                close(client_fd);
                throw SocketException("Invalid IP address");
            }

            if (::connect(client_fd, reinterpret_cast<sockaddr *>(&server_addr), sizeof(server_addr)) == -1 &&
                errno != EINPROGRESS)
            {
                close(client_fd);
                throw SocketException("Failed to connect client socket");
            }

            return client_fd;
        }

        void register_socket_to_epoll(int socket_fd, uint32_t events, const SocketCallbacks &callbacks, bool is_server)
        {
            epoll_event event{};
            event.events = events;
            event.data.fd = socket_fd;

            if (epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, socket_fd, &event) == -1)
            {
                close(socket_fd);
                throw SocketException("Failed to add socket to epoll");
            }

            if (!connections_.try_emplace(socket_fd, Connection{socket_fd, is_server, callbacks}).second)
            {
                close(socket_fd);
                throw SocketException("Failed to add socket to connections");
            }
        }

        void handle_io_event(const epoll_event &event)
        {
            int socket_fd = event.data.fd;
            uint32_t event_mask = event.events;

            auto it = connections_.find(socket_fd);
            if (it == connections_.end())
            {
                throw SocketException("Unknown file descriptor in epoll event");
            }

            auto &connection = it->second;

            if (event_mask & EPOLLIN)
            {
                if (connection.is_server_)
                {
                    accept_new_connection(connection);
                }
                else
                {
                    process_received_data(connection);
                }
            }

            if (event_mask & (EPOLLERR | EPOLLHUP))
            {
                handle_disconnect(connection);
            }
        }

        void accept_new_connection(Connection &server_connection)
        {
            sockaddr_in client_addr{};
            socklen_t client_len = sizeof(client_addr);
            int client_fd = accept(server_connection.socket_fd_, reinterpret_cast<sockaddr *>(&client_addr), &client_len);

            if (client_fd == -1)
            {
                if (errno != EAGAIN && errno != EWOULDBLOCK)
                {
                    std::cerr << "Failed to accept new connection\n";
                }
                return;
            }

            set_socket_non_blocking(client_fd);

            auto &callbacks = server_connection.callbacks_;
            register_socket_to_epoll(client_fd, EPOLLIN | EPOLLET, callbacks, false);
            callbacks.accept_callback_(client_fd);
            callbacks.connect_callback_();
        }

        void process_received_data(Connection &connection)
        {
            ssize_t bytes_read = read(connection.socket_fd_, connection.buffer_, sizeof(connection.buffer_));
            if (bytes_read > 0)
            {
                // Data successfully read
                std::string_view data{connection.buffer_, static_cast<size_t>(bytes_read)};
                if (connection.callbacks_.data_callback_)
                {
                    connection.callbacks_.data_callback_(data);
                }
            }
            else if (bytes_read == 0)
            {
                // Peer has closed the connection
                handle_disconnect(connection);
            }
            else if (bytes_read == -1)
            {
                if (errno == EAGAIN || errno == EWOULDBLOCK)
                {
                    // No data available right now, continue polling
                    return;
                }
                else
                {
                    // Real error occurred
                    std::cerr << "Failed to read data\n";
                    handle_disconnect(connection);
                }
            }
        }

        void handle_disconnect(Connection &connection)
        {
            remove_socket(connection.socket_fd_);
            if (connection.callbacks_.disconnect_callback_)
            {
                connection.callbacks_.disconnect_callback_();
            }
        }

        void remove_socket(int socket_fd)
        {
            if (epoll_ctl(epoll_fd_, EPOLL_CTL_DEL, socket_fd, nullptr) == -1)
            {
                std::cerr << "Failed to remove socket_fd from epoll\n";
            }
            close(socket_fd);
            connections_.erase(socket_fd);
        }

        void set_socket_non_blocking(int socket_fd)
        {
            int flags = fcntl(socket_fd, F_GETFL, 0);
            if (flags == -1 || fcntl(socket_fd, F_SETFL, flags | O_NONBLOCK) == -1)
            {
                throw SocketException("Failed to set socket to non-blocking");
            }
        }
    };
}
