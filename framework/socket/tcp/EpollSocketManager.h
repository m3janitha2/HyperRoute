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
#include <boost/lockfree/spsc_queue.hpp>
#include <framework/socket/tcp/SocketException.h>

namespace hyper::framework
{
    class TransportSingleThreaded;

    class EpollSocketManager
    {
    public:
        ~EpollSocketManager();

        EpollSocketManager(const EpollSocketManager &) = delete;
        EpollSocketManager &operator=(const EpollSocketManager &) = delete;

        static EpollSocketManager &instance()
        {
            static EpollSocketManager instance;
            return instance;
        }

        void connect_server(TransportSingleThreaded &transport);
        void disconnect_server(TransportSingleThreaded &transport);
        void connect_client(TransportSingleThreaded &transport);
        void disconnect_client(TransportSingleThreaded &transport);
        void run();
        bool send_data(int fd, std::string_view data);
        bool send_data_async(int fd, std::string_view data);

    private:
        EpollSocketManager();

        void add_tcp_server(TransportSingleThreaded &transport);
        void add_tcp_client(TransportSingleThreaded &transport);
        int create_tcp_server_socket(const std::string &local_ip, std::uint16_t local_port);
        int create_tcp_client_socket(const std::string &remote_ip, std::uint16_t remote_port,
                                     const std::string &local_ip, std::uint16_t local_port);
        void register_socket_to_epoll(int socket_fd, uint32_t events, TransportSingleThreaded &transport);
        void process_io_events();
        void handle_io_event(const epoll_event &event);
        void accept_new_connection(TransportSingleThreaded &transport);
        void process_received_data(TransportSingleThreaded &transport);
        void process_async_send();
        void handle_disconnect(TransportSingleThreaded &transport, const std::string &error);
        void remove_socket(int socket_fd);
        void set_socket_options(int socket_fd);
        void set_socket_non_blocking(int socket_fd);

        int epoll_fd_;
        inline static constexpr std::size_t ASYNC_QUEUE_SIZE{1024};
        boost::lockfree::spsc_queue<std::pair<int, std::string_view>, boost::lockfree::capacity<ASYNC_QUEUE_SIZE>> async_send_queue_;
    };
}