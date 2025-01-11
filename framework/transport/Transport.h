#pragma once
#include <framework/utility/RejectInfo.h>
#include <functional>
#include <string_view>
#include <iostream>

namespace hyper::framework
{
    /* Transport Abstraction */
    /* - Sends messages to the wire and receives messages from the wire. */
    /* - Implements protocols such as TCP, TCPDirect, UDP, etc. */
    /* - Defines and uses a threading policy (e.g., single-threaded or thread pool). */
    
    template <typename T>
    concept TransportCallbacksInf = requires(T t, std::string_view data) {
        { t.on_connect() } -> std::same_as<void>;
        { t.on_disconnect() } -> std::same_as<void>;
        { t.on_data(data) } -> std::same_as<std::size_t>;
    };

    struct TransportCallbacks
    {
        explicit TransportCallbacks(std::function<void()> connect_callback,
                                    std::function<void()> disconnect_callback,
                                    std::function<std::size_t(std::string_view data)> data_callback)
            : connect_callback_{connect_callback},
              disconnect_callback_{disconnect_callback},
              data_callback_{data_callback} {}

        std::function<void()> connect_callback_{};
        std::function<void()> disconnect_callback_{};
        std::function<std::size_t(std::string_view data)> data_callback_{};
    };

    struct Transport
    {
    public:
        explicit Transport(const TransportCallbacks &transport_callbacks)
            : transport_callbacks_{transport_callbacks} {}

        void connect() { transport_callbacks_.connect_callback_(); }
        void disconnect() { transport_callbacks_.disconnect_callback_(); }
        std::size_t on_data(std::string_view data) { return transport_callbacks_.data_callback_(data); }

        RejectInfo send_data(std::string_view data)
        {
            receive_data_cb_for_test_(data);
            return RejectInfo{};
        }

        void set_receive_data_cb_for_test(const std::function<void(std::string_view data)> &cb)
        {
            receive_data_cb_for_test_ = cb;
        }

    private:
        const TransportCallbacks &transport_callbacks_;
        std::function<void(std::string_view data)> receive_data_cb_for_test_;
    };

    /* may be use a ring buffer */
    /* copy data from socket buffer */
    /* adjust offsets */
}