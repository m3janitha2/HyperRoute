#pragma once

#include <iostream>
#include <cstring>
#include <string_view>
#include <functional>
#include <algorithm>
#include <framework/socket/tcp/EpollSocketManager.h>

namespace hyper::framework
{
    class TCPSimulator
    {
    public:
        enum class Type
        {
            Server,
            Client
        };

        explicit TCPSimulator(const std::string &name,
                              const std::string &ip, int port, Type type) noexcept
            : name_(name), ip_(ip), port_(port)
        {
            SocketCallbacks transport_callbacks{
                [this]()
                { on_connect(); }, [this]()
                { on_disconnect(); }, [this](std::string_view data)
                { return on_data(data); }, [this](int socket_fd)
                { on_connection_accept(socket_fd); }};
            if (type == Type::Server)
                socket_fd_ = socket_manager_.add_server(ip, port, transport_callbacks);
            else
                socket_fd_ = socket_manager_.add_client(ip, port, transport_callbacks);
        }

        void run()
        {
            socket_manager_.run();
        }

        void on_connect()
        {
            std::cout << name_ << " Connected" << std::endl;
        }

        void on_disconnect()
        {
            std::cout << name_ << " Disconnected" << std::endl;
        }

        void on_connection_accept(int socket_fd)
        {
            socket_fd_ = socket_fd;
        }

        std::size_t on_data(std::string_view data)
        {
            try
            {
                std::cout << name_ << " received: " << data << std::endl;
                std::this_thread::sleep_for(std::chrono::seconds(5));
                socket_manager_.send_data(socket_fd_, data);
                std::cout << name_ << " sent: " << data << std::endl;
                return 0;
            }
            catch (std::exception &err)
            {
                std::cerr << "error: " << err.what() << std::endl;
            }
            return 0;
        }

        void send_data(std::string_view data) noexcept
        {
            socket_manager_.send_data(socket_fd_, data);
        }

        void print_statistics() const noexcept
        {
        }

    private:
        const std::string name_{};
        const std::string ip_{};
        const int port_{};
        EpollSocketManager socket_manager_{};
        int socket_fd_{};
        char buffer_[1024]{};
    };
}
