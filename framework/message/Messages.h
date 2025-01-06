#pragma once

#include <framework/utility/Types.h>
#include <string_view>
#include <chrono>
#include <ostream>

namespace max::framework::message
{
    /* Message interface used by the framework framework */

    using Timestamp = std::chrono::time_point<std::chrono::steady_clock>;
    using TimestampClock = std::chrono::steady_clock;

    class Message
    {
    public:
        Message() = default;
        Message(const Message &) = delete;

        [[nodiscard]] constexpr UID uid() const noexcept { return uid_; }
        constexpr void uid(UID uid) noexcept { uid_ = uid; }

        [[nodiscard]] constexpr auto in_time() const noexcept { return in_time_.time_since_epoch().count(); }
        constexpr void in_time(Timestamp in_time) noexcept { in_time_ = in_time; }
        void update_in_time() noexcept { in_time(TimestampClock::now()); }

        [[nodiscard]] constexpr auto out_time() const noexcept { return out_time_.time_since_epoch().count(); }
        constexpr void out_time(Timestamp out_time) noexcept { out_time_ = out_time; }
        void update_out_time() noexcept { out_time(TimestampClock::now()); }

        constexpr auto latency_in_ns() noexcept { return std::chrono::duration_cast<std::chrono::nanoseconds>(out_time_ - in_time_).count(); }

    private:
        UID uid_{};
        Timestamp in_time_{TimestampClock::now()};
        Timestamp out_time_{};
    };

    struct FirstEvent
    {
    }; /* First Message of the message chain */
    struct SubsequentEvent
    {
    }; /* Subsequent Messages of the same message chain */

    struct NewOrderSingle : public Message, FirstEvent
    {
    };

    struct CancelReplaceRequest : public Message, SubsequentEvent
    {
    };

    struct CancelRequest : public Message, SubsequentEvent
    {
    };

    struct ExecutionReport : public Message, SubsequentEvent
    {
    };

    struct CancelReject : public Message, SubsequentEvent
    {
    };
}