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

        [[nodiscard]] constexpr auto in_timestamp() const noexcept { return in_timestamp_.time_since_epoch().count(); }
        constexpr void in_timestamp(Timestamp in_timestamp) noexcept { in_timestamp_ = in_timestamp; }
        void update_in_timestamp() noexcept { in_timestamp(TimestampClock::now()); }

        [[nodiscard]] constexpr auto out_timestamp() const noexcept { return out_timestamp_.time_since_epoch().count(); }
        constexpr void out_timestamp(Timestamp out_timestamp) noexcept { out_timestamp_ = out_timestamp; }
        void update_out_timestamp() noexcept { out_timestamp(TimestampClock::now()); }

        constexpr auto latency_in_ns() noexcept { return std::chrono::duration_cast<std::chrono::nanoseconds>(out_timestamp_ - in_timestamp_).count(); }

    private:
        UID uid_{};
        Timestamp in_timestamp_{TimestampClock::now()};
        Timestamp out_timestamp_{};
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