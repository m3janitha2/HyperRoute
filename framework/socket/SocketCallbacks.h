#pragma once

#include <functional>
#include <string_view>
#include <stdexcept>

namespace hyper::framework
{
    struct SocketCallbacks
    {
        explicit SocketCallbacks(std::function<void(int)> connect_callback,
                                 std::function<void(const std::string &)> disconnect_callback,
                                 std::function<std::size_t(std::string_view)> data_callback)
            : connect_callback_{std::move(connect_callback)},
              disconnect_callback_{std::move(disconnect_callback)},
              data_callback_{std::move(data_callback)}
        {
            if (!connect_callback_ ||
                !disconnect_callback_ ||
                !data_callback_)
            {
                throw std::invalid_argument("All transport callbacks must be set and non-null.");
            }
        }

        std::function<void(int)> connect_callback_;
        std::function<void(const std::string &)> disconnect_callback_;
        std::function<std::size_t(std::string_view)> data_callback_;
    };
}