#pragma once

#include <iostream>
#include <cstring>
#include <string_view>
#include <functional>
#include <algorithm>
#include <thread>
#include <chrono>
#include <framework/transport/TransportSingleThreaded.h>
#include <framework/socket/tcp/EpollSocketManager.h>

namespace hyper::framework
{
    class TCPSimulator
    {
    public:
        explicit TCPSimulator(const std::string &name,
                              const Configuration &config) noexcept
            : name_(name),
              transport_{config,
                         TransportCallbacks{
                             [this]() noexcept
                             { on_connect(); },
                             [this]() noexcept
                             { on_disconnect(); },
                             [this](std::string_view data) noexcept
                             { return on_data(data); }}}
        {
        }

        void run()
        {
            auto &socket_mgr = EpollSocketManager::instance();
            socket_mgr.run();
        }

        RejectInfo connect() noexcept
        try
        {
            return transport_.connect();
        }
        catch (const std::exception &e)
        {
            std::cerr << "Failed to connect: " << e.what() << '\n';
            return RejectInfo(e.what(), InteranlRejectCode::Transport_Failed_To_Send_data);
        }

        RejectInfo disconnect() noexcept
        try
        {

            return transport_.disconnect();
        }
        catch (const std::exception &e)
        {
            std::cerr << "Failed to disconnect: " << e.what() << '\n';
            return RejectInfo(e.what(), InteranlRejectCode::Transport_Failed_To_Send_data);
        }

        void on_connect()
        {
            std::cout << name_ << " Connected." << std::endl;
        }

        void on_disconnect()
        {
            std::cout << name_ << " Disconnected." << std::endl;
        }

        std::size_t on_data(std::string_view data)
        {
            std::cout << name_ << " Received:" << data << std::endl;
            try
            {
                std::this_thread::sleep_for(std::chrono::seconds(5));
                send_data(data);
            }
            catch (const std::exception &e)
            {
                std::cerr << "Failed to seend: " << e.what() << '\n';
                return 0;
            }
            return data.length();
        }

        void send_data(std::string_view data) noexcept
        {
            if (auto reject_info = transport_.send_data(data);
                reject_info == true)
                std::cout << name_ << " Sent:" << data << std::endl;
        }

    private:
        const std::string name_{};
        TransportSingleThreaded transport_;
    };
}
