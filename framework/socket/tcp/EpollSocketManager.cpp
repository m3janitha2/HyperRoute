#include <framework/socket/tcp/EpollSocketManager.h>
#include <framework/transport/TransportSingleThreaded.h>

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
        close(epoll_fd_);
    }

    void EpollSocketManager::connect_server(TransportSingleThreaded &transport)
    {
        add_tcp_server(transport);
    }

    void EpollSocketManager::disconnect_server(TransportSingleThreaded &transport)
    {
        remove_socket(transport.server_fd());
        handle_disconnect(transport,
                          "Disconnect called. socket_fd:" +
                              std::to_string(transport.socket_fd()));
    }

    void EpollSocketManager::connect_client(TransportSingleThreaded &transport)
    {
        add_tcp_client(transport);
    }

    void EpollSocketManager::disconnect_client(TransportSingleThreaded &transport)
    {
        handle_disconnect(transport,
                          "Disconnect called. socket_fd:" +
                              std::to_string(transport.socket_fd()));
    }

    void EpollSocketManager::run()
    {
        process_io_events();
    }

    bool EpollSocketManager::send_data(int fd, std::string_view data)
    {
        auto bytes_sent = send(fd, data.data(), data.size(), 0);
        if (bytes_sent == -1 && errno != EAGAIN)
        {
            std::cerr << "Failed to send data on fd: " << fd << "\n";
            return false;
        }
        return true;
    }

    void EpollSocketManager::add_tcp_server(TransportSingleThreaded &transport)
    {
        auto local_ip = transport.local_ip();
        auto local_port = transport.local_port();

        auto server_fd = create_tcp_server_socket(local_ip, local_port);
        register_socket_to_epoll(server_fd, EPOLLIN | EPOLLET | EPOLLOUT, transport);
        transport.server_fd(server_fd);
    }

    void EpollSocketManager::add_tcp_client(TransportSingleThreaded &transport)
    {
        auto remote_ip = transport.remote_ip();
        auto remote_port = transport.remote_port();
        auto local_ip = transport.local_ip();
        auto local_port = transport.local_port();

        auto client_fd = create_tcp_client_socket(remote_ip, remote_port, local_ip, local_port);
        register_socket_to_epoll(client_fd, EPOLLIN | EPOLLET | EPOLLOUT, transport);
        transport.on_connect(client_fd);
    }

    int EpollSocketManager::create_tcp_server_socket(const std::string &ip, std::uint16_t port)
    {
        auto server_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (server_fd == -1)
        {
            throw SocketException("Failed to create server socket");
        }

        set_socket_options(server_fd);

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

    int EpollSocketManager::create_tcp_client_socket(const std::string &remote_ip, std::uint16_t remote_port,
                                                     const std::string &local_ip, std::uint16_t local_port)
    {
        auto client_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (client_fd == -1)
        {
            throw SocketException("Failed to create client socket");
        }

        set_socket_options(client_fd);

        if (!local_ip.empty() || local_port > 0)
        {
            sockaddr_in local_addr{};
            local_addr.sin_family = AF_INET;
            local_addr.sin_port = htons(local_port);

            if (inet_pton(AF_INET, local_ip.c_str(), &local_addr.sin_addr) <= 0)
            {
                close(client_fd);
                throw SocketException("Invalid local IP address");
            }

            if (bind(client_fd, reinterpret_cast<sockaddr *>(&local_addr), sizeof(local_addr)) == -1)
            {
                close(client_fd);
                throw SocketException("Failed to bind client socket to local IP/port");
            }
        }

        sockaddr_in server_addr{};
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(remote_port);
        if (inet_pton(AF_INET, remote_ip.c_str(), &server_addr.sin_addr) <= 0)
        {
            close(client_fd);
            throw SocketException("Invalid remote IP address");
        }

        if (::connect(client_fd, reinterpret_cast<sockaddr *>(&server_addr), sizeof(server_addr)) == -1 &&
            errno != EINPROGRESS)
        {
            close(client_fd);
            throw SocketException("Failed to connect client socket");
        }

        return client_fd;
    }

    void EpollSocketManager::register_socket_to_epoll(int socket_fd, uint32_t events, TransportSingleThreaded &transport)
    {
        epoll_event event{};
        event.events = events;
        event.data.ptr = &transport;

        if (epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, socket_fd, &event) == -1)
        {
            close(socket_fd);
            throw SocketException("Failed to add socket to epoll");
        }
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

    void EpollSocketManager::handle_io_event(const epoll_event &event)
    {
        auto &transport = *static_cast<TransportSingleThreaded *>(event.data.ptr);
        auto event_mask = event.events;

        if (event_mask & EPOLLIN)
        {
            if (transport.is_server() && !transport.is_accepted()) /* todox: remove is_accepted() */
                accept_new_connection(transport);
            else
                process_received_data(transport);
        }

        if (event_mask & (EPOLLERR | EPOLLHUP))
        {
            handle_disconnect(transport,
                              "Peer has closed the connection. socket_fd:" +
                                  std::to_string(transport.socket_fd()));
        }
    }

    void EpollSocketManager::accept_new_connection(TransportSingleThreaded &transport)
    {
        sockaddr_in client_addr{};
        socklen_t client_len = sizeof(client_addr);
        auto client_fd = accept(transport.server_fd(), reinterpret_cast<sockaddr *>(&client_addr), &client_len);

        if (client_fd == -1)
        {
            if (errno != EAGAIN && errno != EWOULDBLOCK)
            {
                std::cerr << "Failed to accept new connection" << std::endl;
            }
            return;
        }

        set_socket_options(client_fd);
        register_socket_to_epoll(client_fd, EPOLLIN | EPOLLET, transport);
        transport.on_connect(client_fd);
    }

    void EpollSocketManager::process_received_data(TransportSingleThreaded &transport)
    {
        auto bytes_read = transport.read_data();
        if (bytes_read > 0) [[likely]]
        {
            transport.process_data(bytes_read);
        }
        else if (bytes_read == 0) [[unlikely]]
        {
            handle_disconnect(transport,
                              "Peer has closed the connection. socket_fd:" +
                                  std::to_string(transport.socket_fd()));
        }
        else if (bytes_read == -1) [[unlikely]]
        {
            if (errno == EAGAIN || errno == EWOULDBLOCK)
            {
                return;
            }
            else
            {
                std::cerr << "Failed to read data. socket_fd:" << transport.socket_fd() << std::endl;
                handle_disconnect(transport,
                                  "Failed to read data. socket_fd:" +
                                      std::to_string(transport.socket_fd()));
            }
        }
    }

    void EpollSocketManager::handle_disconnect(TransportSingleThreaded &transport, const std::string &error)
    {
        remove_socket(transport.socket_fd());
        transport.on_disconnect(error);
    }

    void EpollSocketManager::remove_socket(int socket_fd)
    {
        if (epoll_ctl(epoll_fd_, EPOLL_CTL_DEL, socket_fd, nullptr) == -1)
        {
            std::cerr << "Failed to remove socket_fd from epoll. socket_fd:" << socket_fd << std::endl;
        }

        close(socket_fd);
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