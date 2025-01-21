#pragma once

#include <iostream>
#include <cstring>
#include <string_view>
#include <functional>
#include <algorithm>

namespace hyper::framework
{
    template <typename Protocol>
    class Simulator
    {
    public:
        explicit Simulator(Protocol &protocol) noexcept : protocol_(protocol)
        {
            protocol_.transport().set_receive_data_cb_for_test(
                [this](std::string_view data) noexcept
                { handle_message_from_protocol(data); });
        }

        void send_request(std::string_view data) noexcept
        {
            reset_message_state();
            ++messages_sent_;
            protocol_.transport().on_data(data);
        }

        template <typename MessageType>
        const MessageType &get_last_message() const noexcept
        {
            return *reinterpret_cast<const MessageType *>(buffer_);
        }

        [[nodiscard]] constexpr bool has_received_message() const noexcept { return message_received_; }

        void print_statistics() const noexcept
        {
            std::cout << "Messages sent: " << messages_sent_ << '\n'
                      << "Messages received: " << messages_received_ << '\n';
        }

    private:
        void reset_message_state() noexcept { message_received_ = false; }

        void handle_message_from_protocol(std::string_view data) noexcept
        {
            message_received_ = true;
            ++messages_received_;
            store_message(data);
        }

        void store_message(std::string_view data) noexcept
        {
            const auto copy_size = std::min(data.size(), sizeof(buffer_) - 1);
            std::memcpy(buffer_, data.data(), copy_size);
        }

        Protocol &protocol_;
        volatile bool message_received_{false};
        std::size_t messages_sent_{0};
        std::size_t messages_received_{0};
        char buffer_[1024]{};
    };
}
