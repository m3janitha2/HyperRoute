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
#include <framework/config/Configuration.h>

namespace hyper::framework
{
    class TCPSimulator
    {
    public:
        explicit TCPSimulator(const Configuration &config,
                              const std::string &name,
                              const std::function<std::size_t(std::string_view)> &data_callback) noexcept
            : transport_{config,
                         TransportCallbacks{
                             [this]() noexcept
                             { on_connect(); },
                             [this]() noexcept
                             { on_disconnect(); },
                             [this](std::string_view data) noexcept
                             { return on_data(data); }}},
              name_(name),
              data_callback_(data_callback)
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
            std::cout << name_ << " Received:" << std::endl;
            return data_callback_(data);
        }

        void send_data(std::string_view data) noexcept
        {
            if (auto reject_info = transport_.send_data(data);
                reject_info == true)
                std::cout << name_ << " Sent:" << std::endl;
        }

        void send_data_async(std::string_view data) noexcept
        {
            if (auto reject_info = transport_.send_data_async(data);
                reject_info == true)
                std::cout << name_ << " Sent Async:" << std::endl;
        }

        static Configuration generate_config(const std::string &local_ip,
                                             std::uint16_t local_port,
                                             const std::string &remote_ip,
                                             std::uint16_t remote_port,
                                             const std::string &socket_type,
                                             const std::string &socket_role)
        {
            Configuration cfg;
            cfg.put("local_ip", local_ip);
            cfg.put("local_port", local_port);
            cfg.put("remote_ip", remote_ip);
            cfg.put("remote_port", remote_port);
            cfg.put("socket_type", socket_type);
            cfg.put("socket_role", socket_role);
            return cfg;
        }

        bool is_server() { return transport_.is_server(); }

    private:
        TransportSingleThreaded transport_;
        const std::string name_{};
        const std::function<std::size_t(std::string_view)> data_callback_;
    };
}
