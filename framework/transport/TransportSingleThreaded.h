#pragma once

#include <framework/transport/TransportCallbacks.h>
#include <framework/utility/RejectInfo.h>
#include <framework/config/Configuration.h>
#include <string_view>
#include <cstdint>
#include <boost/lockfree/spsc_queue.hpp>

namespace hyper::framework
{
    struct TransportSingleThreaded
    {
    public:
        explicit TransportSingleThreaded(const Configuration &config,
                                         TransportCallbacks transport_callbacks);
        ~TransportSingleThreaded();

        TransportSingleThreaded(const TransportSingleThreaded &) = delete;
        TransportSingleThreaded &operator=(const TransportSingleThreaded &) = delete;

        RejectInfo connect() noexcept;
        RejectInfo disconnect() noexcept;

        void on_connect(int socket_fd) noexcept;
        void on_disconnect(const std::string &error) noexcept;
        long read_data();
        void process_data(std::size_t bytes_read) noexcept;
        std::size_t on_data(std::string_view data) const noexcept;
        RejectInfo send_data(std::string_view data) const noexcept;
        RejectInfo send_data_async(std::string_view data) const noexcept;

        [[nodiscard]] constexpr std::size_t length_for_read() const noexcept { return RECEIVE_BUFFER_SIZE - write_offset_; }
        [[nodiscard]] constexpr char *buffer_for_read() noexcept { return receive_buffer_ + write_offset_; }

        [[nodiscard]] constexpr bool is_server() const noexcept { return is_server_; }
        [[nodiscard]] constexpr bool is_accepted() const noexcept { return socket_fd_ != INVALID_FD; }

        [[nodiscard]] constexpr int socket_fd() const noexcept { return socket_fd_; }
        [[nodiscard]] constexpr int server_fd() const noexcept { return server_fd_; }
        constexpr void server_fd(int server_fd) noexcept { server_fd_ = server_fd; }

        [[nodiscard]] constexpr const std::string &remote_ip() const noexcept { return remote_ip_; }
        void remote_ip(std::string_view remote_ip) noexcept { remote_ip_ = remote_ip; }

        [[nodiscard]] constexpr std::uint16_t remote_port() const noexcept { return remote_port_; }
        constexpr void remote_port(std::uint16_t remote_port) noexcept { remote_port_ = remote_port; }

        [[nodiscard]] constexpr const std::string &local_ip() const noexcept { return local_ip_; }
        void local_ip(std::string_view local_ip) noexcept { local_ip_ = local_ip; }

        [[nodiscard]] constexpr std::uint16_t local_port() const noexcept { return local_port_; }
        constexpr void local_port(std::uint16_t local_port) noexcept { local_port_ = local_port; }

        void set_receive_data_cb_for_test(std::function<void(std::string_view)> cb) noexcept;

    private:
        void load_config(const Configuration &config);
        constexpr void clear_receive_buffer() noexcept;

        TransportCallbacks transport_callbacks_;

        inline static constexpr std::size_t RECEIVE_BUFFER_SIZE{1024};
        char receive_buffer_[RECEIVE_BUFFER_SIZE]{};
        std::size_t write_offset_{0};

        inline static constexpr int INVALID_FD{-1};
        int socket_fd_{INVALID_FD};
        int server_fd_{INVALID_FD};

        bool is_server_{false};
        std::string remote_ip_{};
        std::uint16_t remote_port_{0};
        std::string local_ip_{};
        std::uint16_t local_port_{0};

        std::function<void(std::string_view)> receive_data_cb_for_test_;
    };
}
