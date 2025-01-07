#pragma once
#include <framework/protocol/RejectInfo.h>
#include <functional>
#include <string_view>
#include <iostream>

namespace hyper::framework
{
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
            std::cout << "received data" << std::endl;
            return RejectInfo{};
        }

    private:
        const TransportCallbacks &transport_callbacks_;
    };

    
        /* may be use a ring buffer */
        /* copy data from socket buffer */
        /* adjust offsets */
}