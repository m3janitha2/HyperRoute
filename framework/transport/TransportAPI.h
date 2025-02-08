#pragma once
#include <framework/config/Configuration.h>
#include <framework/transport/TransportCallbacks.h>
#include <framework/utility/RejectInfo.h>
#include <framework/transport/TransportSingleThreaded.h>
#include <string_view>

namespace hyper::framework
{
    /* For testing the framework */
    class TransportAPI
    {
    public:
        explicit TransportAPI(const Configuration &config,
                           TransportCallbacks transport_callbacks)
            : config_(config),
              transport_callbacks_{std::move(transport_callbacks)} {}

        TransportAPI(const TransportAPI &) = delete;
        TransportAPI &operator=(const TransportAPI &) = delete;

        void connect() noexcept { transport_callbacks_.connect_callback_(); }
        void disconnect() noexcept { transport_callbacks_.disconnect_callback_(); }

        std::size_t on_data(std::string_view data, Timestamp timestamp) noexcept;
        RejectInfo send_data(std::string_view data) noexcept;

        void set_receive_data_cb_for_test(std::function<void(std::string_view)> cb) noexcept;

    private:
        const Configuration config_{};
        TransportCallbacks transport_callbacks_;
        std::function<void(std::string_view)> receive_data_cb_for_test_;
    };
}