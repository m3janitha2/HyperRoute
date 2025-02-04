#pragma once
#include <framework/config/Configuration.h>
#include <framework/transport/TransportCallbacks.h>
#include <framework/utility/RejectInfo.h>
#include <framework/transport/TransportSingleThreaded.h>
#include <string_view>

namespace hyper::framework
{
    using Transport = TransportSingleThreaded;

    // struct Transport
    // {
    // public:
    //     explicit Transport(const Configuration &config,
    //                        TransportCallbacks transport_callbacks)
    //         : config_(config),
    //           transport_callbacks_{std::move(transport_callbacks)} {}

    //     Transport(const Transport &) = delete;
    //     Transport &operator=(const Transport &) = delete;

    //     void connect() noexcept { transport_callbacks_.connect_callback_(); }
    //     void disconnect() noexcept { transport_callbacks_.disconnect_callback_(); }

    //     std::size_t on_data(std::string_view data) noexcept { return transport_callbacks_.data_callback_(data); }
    //     RejectInfo send_data(std::string_view data) noexcept;

    //     void set_receive_data_cb_for_test(std::function<void(std::string_view)> cb) noexcept;

    // private:
    //     const Configuration config_{};
    //     TransportCallbacks transport_callbacks_;
    //     std::function<void(std::string_view)> receive_data_cb_for_test_;
    // };
}

/* may be use a ring buffer */
/* copy data from socket buffer */
/* adjust offsets */
