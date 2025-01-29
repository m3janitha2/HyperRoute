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
#include <netinet/in.h> 
#include <netinet/tcp.h>
#include <unistd.h>
#include <fcntl.h>
#include <mutex>
#include <queue>
#include <vector>
#include <optional>
#include <framework/socket/SocketCallbacks.h>
#include <framework/socket/tcp/SocketException.h>

namespace hyper::framework
{
    class EpollSocketManager
    {
    public:
        EpollSocketManager();
        ~EpollSocketManager();

        EpollSocketManager(const EpollSocketManager &) = delete;
        EpollSocketManager &operator=(const EpollSocketManager &) = delete;

        void add_tcp_server(const std::string &ip, int port, const SocketCallbacks &callbacks);
        void add_tcp_client(const std::string &ip, int port, const SocketCallbacks &callbacks);

        void run();
        void send_data(int fd, std::string_view data);

    private:
        struct Connection
        {
            Connection(int socket_fd, bool is_server, SocketCallbacks callbacks)
                : socket_fd_(socket_fd), is_server_(is_server), callbacks_(callbacks) {}

            int socket_fd_;
            bool is_server_;
            SocketCallbacks callbacks_;
            char buffer_[4096]{};
        };

        int epoll_fd_;
        std::unordered_map<int, Connection> connections_;

        int create_tcp_server_socket(const std::string &ip, int port);
        int create_tcp_client_socket(const std::string &ip, int port);
        void register_socket_to_epoll(int socket_fd, uint32_t events,
                                      const SocketCallbacks &callbacks, bool is_server);

        void process_io_events();
        void handle_io_event(const epoll_event &event);
        void accept_new_connection(Connection &server_connection);
        void process_received_data(Connection &connection);
        void handle_disconnect(Connection &connection, const std::string &error);
        void remove_socket(int socket_fd);
        void set_socket_options(int socket_fd);
        void set_socket_non_blocking(int socket_fd);
    };
}