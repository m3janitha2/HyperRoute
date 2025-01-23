#pragma once
#include <framework/utility/RejectInfo.h>
#include <functional>
#include <string_view>
#include <stdexcept>
#include <utility>

namespace hyper::framework
{
    template <typename T>
    concept TransportCallbackInf = requires(T t, std::string_view data) {
        { t.on_connect() } -> std::same_as<void>;
        { t.on_disconnect() } -> std::same_as<void>;
        { t.on_data(data) } -> std::same_as<std::size_t>;
    };

    struct TransportCallbacks
    {
        explicit TransportCallbacks(std::function<void()> connect_callback,
                                    std::function<void()> disconnect_callback,
                                    std::function<std::size_t(std::string_view)> data_callback)
            : connect_callback_{std::move(connect_callback)},
              disconnect_callback_{std::move(disconnect_callback)},
              data_callback_{std::move(data_callback)}
        {
            if (!connect_callback_ || !disconnect_callback_ || !data_callback_)
            {
                throw std::invalid_argument("All transport callbacks must be set and non-null.");
            }
        }

        std::function<void()> connect_callback_;
        std::function<void()> disconnect_callback_;
        std::function<std::size_t(std::string_view)> data_callback_;
    };

    struct Transport
    {
    public:
        explicit Transport(TransportCallbacks transport_callbacks)
            : transport_callbacks_{std::move(transport_callbacks)} {}

        Transport(const Transport &) = delete;
        Transport &operator=(const Transport &) = delete;

        void connect() noexcept { transport_callbacks_.connect_callback_(); }
        void disconnect() noexcept { transport_callbacks_.disconnect_callback_(); }

        std::size_t on_data(std::string_view data) noexcept { return transport_callbacks_.data_callback_(data); }
        RejectInfo send_data(std::string_view data) noexcept;

        void set_receive_data_cb_for_test(std::function<void(std::string_view)> cb) noexcept;

    private:
        TransportCallbacks transport_callbacks_;
        std::function<void(std::string_view)> receive_data_cb_for_test_;
    };
}

/* may be use a ring buffer */
/* copy data from socket buffer */
/* adjust offsets */
