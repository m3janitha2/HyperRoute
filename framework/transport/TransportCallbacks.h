#pragma once

#include <framework/utility/TimeAux.h>
#include <functional>
#include <string_view>
#include <stdexcept>

namespace hyper::framework
{
    template <typename T>
    concept TransportCallbackInf = requires(T t, std::string_view data, Timestamp timestamp) {
        { t.on_connect() } -> std::same_as<void>;
        { t.on_disconnect() } -> std::same_as<void>;
        { t.on_data(data, timestamp) } -> std::same_as<std::size_t>;
    };

    struct TransportCallbacks
    {
        explicit TransportCallbacks(std::function<void()> connect_callback,
                                    std::function<void()> disconnect_callback,
                                    std::function<std::size_t(std::string_view, Timestamp)> data_callback)
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
        std::function<std::size_t(std::string_view, Timestamp)> data_callback_;
    };
}