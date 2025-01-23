#pragma once

#include <framework/utility/CrtpBase.h>
#include <framework/utility/Types.h>
#include <string_view>
#include <chrono>
#include <type_traits>
#include <concepts>

namespace hyper::framework
{
    using TimestampClock = std::chrono::high_resolution_clock;
    using Timestamp = std::chrono::time_point<TimestampClock>;

    /* Minimum message interface required by the framework */
    template <typename Msg>
    concept MessageInf = requires(Msg msg, Timestamp timestamp) {
        { msg.uid() } -> std::same_as<UID>;
        { msg.in_timestamp() };
        { msg.in_timestamp(timestamp) } -> std::same_as<void>;
        { msg.out_timestamp() };
        { msg.out_timestamp(timestamp) } -> std::same_as<void>;
        { msg.latency_in_ns() };
        /* Implemented in the derived message type */
        { msg.msg() };
        { msg.data() } -> std::same_as<std::string_view>;
    };

    class Message
    {
    public:
        explicit Message(Timestamp in_timestamp = TimestampClock::now())
            : in_timestamp_(in_timestamp) {}

        Message(const Message &) = delete;
        Message &operator=(const Message &) = delete;

        [[nodiscard]] constexpr UID uid() const noexcept { return uid_; }
        constexpr void uid(UID uid) noexcept { uid_ = uid; }

        [[nodiscard]] constexpr auto in_timestamp() const noexcept { return in_timestamp_; }
        constexpr void in_timestamp(Timestamp in_timestamp) noexcept { in_timestamp_ = in_timestamp; }
        void update_in_timestamp() noexcept { in_timestamp(TimestampClock::now()); }

        [[nodiscard]] constexpr auto out_timestamp() const noexcept { return out_timestamp_; }
        constexpr void out_timestamp(Timestamp out_timestamp) noexcept { out_timestamp_ = out_timestamp; }
        void update_out_timestamp() noexcept { out_timestamp(TimestampClock::now()); }

        [[nodiscard]] constexpr auto latency_in_ns() const noexcept { return std::chrono::duration_cast<std::chrono::nanoseconds>(out_timestamp_ - in_timestamp_).count(); }

    private:
        UID uid_{};
        Timestamp in_timestamp_{};
        Timestamp out_timestamp_{};
    };

    struct FirstEvent
    {
    }; /* First Message of the message chain */

    struct SubsequentEvent
    {
    }; /* Subsequent Messages of the same message chain */
}