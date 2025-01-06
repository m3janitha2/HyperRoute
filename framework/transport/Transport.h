#pragma once
#include <framework/protocol/RejectInfo.h>
#include <functional>
#include <string_view>
#include <iostream>

namespace max::framework
{
    struct TransportCallbacks
    {
        explicit TransportCallbacks(std::function<void()> connect_callback,
                                    std::function<void()> disconnect_callback,
                                    std::function<std::size_t(std::string_view data)> data_callback)
            : connect_callback_{connect_callback},
              disconnect_callback_{disconnect_callback},
              data_callback_{data_callback_} {}

        std::function<void()> connect_callback_{};
        std::function<void()> disconnect_callback_{};
        std::function<void()> data_callback_{};
    };

    struct Transport
    {
    public:
        explicit Transport(TransportCallbacks &transport_callbacks)
            : transport_callbacks_{transport_callbacks_} {}

        void connect() { transport_callbacks_.connect_callback_(); }
        void disconnect() { transport_callbacks_.disconnect_callback_(); }
        std::size_t on_data(std::string_view data) { return 0; }
        
        RejectInfo send_data(std::string_view data)
        {
            std::cout << "received data" << std::endl;
            return RejectInfo{};
        }

    private:
        TransportCallbacks &transport_callbacks_;
    };
}