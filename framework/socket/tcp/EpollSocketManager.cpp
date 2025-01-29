#include <framework/socket/tcp/EpollSocketManager.h>

namespace hyper::framework
{

    EpollSocketManager::EpollSocketManager()
    {
        epoll_fd_ = epoll_create1(0);
        if (epoll_fd_ == -1)
        {
            throw SocketException("Failed to create epoll instance");
        }
    }

    EpollSocketManager::~EpollSocketManager()
    {
        for (auto &[fd, _] : connections_)
        {
            close(fd);
        }
        close(epoll_fd_);
    }

    void EpollSocketManager::add_tcp_server(const std::string &ip, int port, const SocketCallbacks &callbacks)
    {
        auto server_fd = create_tcp_server_socket(ip, port);
        register_socket_to_epoll(server_fd, EPOLLIN | EPOLLET | EPOLLOUT, callbacks, true);
    }

    void EpollSocketManager::add_tcp_client(const std::string &ip, int port, const SocketCallbacks &callbacks)
    {
        auto client_fd = create_tcp_client_socket(ip, port);
        register_socket_to_epoll(client_fd, EPOLLIN | EPOLLET | EPOLLOUT, callbacks, false);
        callbacks.connect_callback_(client_fd);
    }

    void EpollSocketManager::process_io_events()
    {
        constexpr int max_events = 1024;
        epoll_event events[max_events];

        while (true)
        {
            auto event_count = epoll_wait(epoll_fd_, events, max_events, -1);
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

    void EpollSocketManager::send_data(int fd, std::string_view data)
    {
        auto bytes_sent = send(fd, data.data(), data.size(), 0);
        if (bytes_sent == -1 && errno != EAGAIN)
        {
            std::cerr << "Failed to send data on fd: " << fd << "\n";
        }
    }

    void EpollSocketManager::run()
    {
        process_io_events();
    }

    int EpollSocketManager::create_tcp_server_socket(const std::string &ip, int port)
    {
        auto server_fd = socket(AF_INET, SOCK_STREAM, 0);
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

    int EpollSocketManager::create_tcp_client_socket(const std::string &ip, int port)
    {
        auto client_fd = socket(AF_INET, SOCK_STREAM, 0);
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

    void EpollSocketManager::register_socket_to_epoll(int socket_fd, uint32_t events, const SocketCallbacks &callbacks, bool is_server)
    {
        if (!connections_.try_emplace(socket_fd, socket_fd, is_server, callbacks).second)
        {
            close(socket_fd);
            throw SocketException("Failed to add socket to connections");
        }

        auto &stored_connection = connections_.at(socket_fd);
        epoll_event event{};
        event.events = events;
        event.data.ptr = &stored_connection;

        if (epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, socket_fd, &event) == -1)
        {
            close(socket_fd);
            connections_.erase(socket_fd);
            throw SocketException("Failed to add socket to epoll");
        }
    }

    void EpollSocketManager::handle_io_event(const epoll_event &event)
    {
        auto &connection = *static_cast<Connection *>(event.data.ptr);
        auto event_mask = event.events;

        if (event_mask & EPOLLIN)
        {
            if (connection.is_server_)
                accept_new_connection(connection);
            else
                process_received_data(connection);
        }

        if (event_mask & (EPOLLERR | EPOLLHUP))
        {
            handle_disconnect(connection,
                              "Peer has closed the connection. socket_fd:" +
                                  std::to_string(connection.socket_fd_));
        }
    }

    void EpollSocketManager::accept_new_connection(Connection &server_connection)
    {
        sockaddr_in client_addr{};
        socklen_t client_len = sizeof(client_addr);
        auto client_fd = accept(server_connection.socket_fd_, reinterpret_cast<sockaddr *>(&client_addr), &client_len);

        if (client_fd == -1)
        {
            if (errno != EAGAIN && errno != EWOULDBLOCK)
            {
                std::cerr << "Failed to accept new connection" << std::endl;
            }
            return;
        }

        set_socket_non_blocking(client_fd);

        auto &callbacks = server_connection.callbacks_;
        register_socket_to_epoll(client_fd, EPOLLIN | EPOLLET, callbacks, false);
        callbacks.connect_callback_(client_fd);
    }

    void EpollSocketManager::process_received_data(Connection &connection)
    {
        auto bytes_read = read(connection.socket_fd_, connection.buffer_, sizeof(connection.buffer_));
        if (bytes_read > 0)
        {
            std::string_view data{connection.buffer_, static_cast<size_t>(bytes_read)};
            connection.callbacks_.data_callback_(data);
        }
        else if (bytes_read == 0)
        {
            handle_disconnect(connection,
                              "Peer has closed the connection. socket_fd:" +
                                  std::to_string(connection.socket_fd_));
        }
        else if (bytes_read == -1)
        {
            if (errno == EAGAIN || errno == EWOULDBLOCK)
            {
                // No data available, continue polling
                return;
            }
            else
            {
                std::cerr << "Failed to read data. socket_fd:" << connection.socket_fd_ << std::endl;
                handle_disconnect(connection,
                                  "Failed to read data. socket_fd:" +
                                      std::to_string(connection.socket_fd_));
            }
        }
    }

    void EpollSocketManager::handle_disconnect(Connection &connection, const std::string &error)
    {
        remove_socket(connection.socket_fd_);
        connection.callbacks_.disconnect_callback_(error);
    }

    void EpollSocketManager::remove_socket(int socket_fd)
    {
        if (epoll_ctl(epoll_fd_, EPOLL_CTL_DEL, socket_fd, nullptr) == -1)
        {
            std::cerr << "Failed to remove socket_fd from epoll. socket_fd:" << socket_fd << std::endl;
        }

        close(socket_fd);
        connections_.erase(socket_fd);
    }

    void EpollSocketManager::set_socket_options(int socket_fd)
    {
        set_socket_non_blocking(socket_fd);

        int flag = 1;

        if (setsockopt(socket_fd, IPPROTO_TCP, TCP_NODELAY, &flag, sizeof(flag)) == -1)
        {
            close(socket_fd);
            throw SocketException("Failed to set TCP_NODELAY");
        }

        if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag)) == -1)
        {
            close(socket_fd);
            throw SocketException("Failed to set SO_REUSEADDR");
        }
    }

    void EpollSocketManager::set_socket_non_blocking(int socket_fd)
    {
        auto flags = fcntl(socket_fd, F_GETFL, 0);
        if (flags == -1 || fcntl(socket_fd, F_SETFL, flags | O_NONBLOCK) == -1)
        {
            close(socket_fd);
            throw SocketException("Failed to add socket to connections");
        }
    }
}