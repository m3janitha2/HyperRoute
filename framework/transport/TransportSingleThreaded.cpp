#include <framework/transport/TransportSingleThreaded.h>
#include <framework/socket/tcp/EpollSocketManager.h>
#include "TransportSingleThreaded.h"

namespace hyper::framework
{
    TransportSingleThreaded::TransportSingleThreaded(const Configuration &config,
                                                     TransportCallbacks transport_callbacks)
        : transport_callbacks_{std::move(transport_callbacks)}
    {
        load_config(config);

        // Connect or listen on startup for simplicity
        if (auto ret = connect(); ret != true)
            std::cerr << "Failed to connec." << std::endl;
    }

    TransportSingleThreaded::~TransportSingleThreaded()
    {
        if (socket_fd_ != INVALID_FD)
        {
            if (auto reject_info = disconnect(); reject_info != true)
                std::cerr << reject_info << std::endl;
        }
    }

    RejectInfo TransportSingleThreaded::connect() noexcept
    try
    {
        if (is_server_)
            EpollSocketManager::instance().connect_server(*this);
        else
            EpollSocketManager::instance().connect_client(*this);

        return RejectInfo{};
    }
    catch (std::exception &err)
    {
        return RejectInfo{err.what(), InteranlRejectCode::Transport_Failed_To_Connect};
    }

    RejectInfo TransportSingleThreaded::disconnect() noexcept
    try
    {
        if (is_server_)
            EpollSocketManager::instance().disconnect_server(*this);
        else
            EpollSocketManager::instance().disconnect_client(*this);

        return RejectInfo{};
    }
    catch (std::exception &err)
    {
        return RejectInfo{err.what(), InteranlRejectCode::Transport_Failed_To_Disconnect};
    }

    void TransportSingleThreaded::on_connect(int socket_fd) noexcept
    {
        socket_fd_ = socket_fd;
        std::cout << "socket connected. socket fd: " << socket_fd
                  << " remote_ip:" << remote_ip_ << " remote_port:" << remote_port_
                  << " local_ip:" << local_ip_ << " local_port:" << local_port_ << std::endl;
        transport_callbacks_.connect_callback_();
    }

    void TransportSingleThreaded::on_disconnect(const std::string &error) noexcept
    {
        socket_fd_ = INVALID_FD;
        std::cout << "socket disconnected. socket fd: " << socket_fd_
                  << " ip:" << remote_ip_ << " port:" << remote_port_
                  << " erorr: " << std::endl;
        clear_receive_buffer();
        transport_callbacks_.disconnect_callback_();
    }

    int TransportSingleThreaded::read_data()
    {
        /* direct read to avoid any copies */
        return read(socket_fd_, buffer_for_read(), length_for_read());
    }

    void TransportSingleThreaded::process_data(std::size_t bytes_read) noexcept
    {
        /* next point write */
        write_offset_ += bytes_read;

        /* consume buffer */
        /* todox: reduce branching */
        while (read_offset_ < RECEIVE_BUFFER_SIZE)
        {
            auto data_lenght = write_offset_ - read_offset_;
            std::string_view data{receive_buffer_ + read_offset_, data_lenght};
            auto bytes_consumed = transport_callbacks_.data_callback_(data);
            if (bytes_consumed == 0)
                break;
            if (bytes_consumed > data_lenght) [[unlikely]]
            {
                std::cerr << "critical error" << std::endl;
            }

            /* consume next message */
            read_offset_ += bytes_consumed;
            if (read_offset_ == write_offset_)
                break;
        }

        /* move remaining unprocessed data to front */
        auto data_length = write_offset_ - read_offset_;
        if (data_length > 0)
            std::memmove(receive_buffer_, receive_buffer_ + read_offset_, data_length);
        read_offset_ = 0;
        write_offset_ = data_length;
    }

    std::size_t TransportSingleThreaded::on_data(std::string_view data)
    {
        /* only for test */
        return transport_callbacks_.data_callback_(data);
    }

    RejectInfo TransportSingleThreaded::send_data(std::string_view data) noexcept
    try
    {
        auto &socket_mgr = EpollSocketManager::instance();
        if (socket_mgr.send_data(socket_fd_, data) != true) [[unlikely]]
            RejectInfo{"Failed to send data on wire", InteranlRejectCode::Transport_Failed_To_Send_data};

        return RejectInfo{};
    }
    catch (std::exception &err)
    {
        return RejectInfo{err.what(), InteranlRejectCode::Transport_Failed_To_Send_data};
    }

    /* Ensure that the data (message) outlives process_async_send */
    RejectInfo TransportSingleThreaded::send_data_async(std::string_view data) noexcept
    try
    {
        auto &socket_mgr = EpollSocketManager::instance();
        if (socket_mgr.send_data_async(socket_fd_, data) != true) [[unlikely]]
            RejectInfo{"Failed to async send data", InteranlRejectCode::Transport_Failed_To_Send_data};

        return RejectInfo{};
    }
    catch (std::exception &err)
    {
        return RejectInfo{err.what(), InteranlRejectCode::Transport_Failed_To_Send_data};
    }

    void TransportSingleThreaded::load_config(const Configuration &config)
    {
        auto socket_type = config.get<std::string>("socket_type");
        auto socket_role = config.get<std::string>("socket_role");
        if (socket_role == "server")
        {
            is_server_ = true;
            local_ip_ = config.get<std::string>("local_ip");
            local_port_ = config.get<std::uint16_t>("local_port");
        }
        else if (socket_role == "client")
        {
            local_ip_ = config.get<std::string>("local_ip");
            local_port_ = config.get<std::uint16_t>("local_port");
            remote_ip_ = config.get<std::string>("remote_ip");
            remote_port_ = config.get<std::uint16_t>("remote_port");
        }
    }

    constexpr void TransportSingleThreaded::clear_receive_buffer() noexcept
    {
        write_offset_ = 0;
        read_offset_ = 0;
    }

    void TransportSingleThreaded::set_receive_data_cb_for_test(std::function<void(std::string_view)> cb) noexcept
    {
        receive_data_cb_for_test_ = std::move(cb);
    }
}