#pragma once

#include <application/message/core/Types.h>
#include <ostream>
#include <string_view>
#include <chrono>

namespace max::message::core
{
    /* Message interface used by the core framework */
    
    using Timestamp = std::chrono::time_point<std::chrono::steady_clock>;
    using TimestampClock = std::chrono::steady_clock;

    class Message
    {
    public:
        [[nodiscard]] constexpr UID uid() const noexcept { return uid_; }
        constexpr void uid(UID uid) noexcept { uid_ = uid; }

        [[nodiscard]] constexpr Timestamp in_time() const noexcept { return in_time_; }
        constexpr void in_time(Timestamp in_time) noexcept { in_time_ = in_time; }

        [[nodiscard]] constexpr Timestamp out_time() const noexcept { return out_time_; }
        constexpr void out_time(Timestamp out_time) noexcept { out_time_ = out_time; }

    private:
        UID uid_{};
        Timestamp in_time_{TimestampClock::now()};
        Timestamp out_time_{};
    };

    struct NewOrderSingle : public Message
    {
    };

    struct CancelReplaceRequest : public Message
    {
    };

    struct CancelRequest : public Message
    {
    };

    struct ExecutionReport : public Message
    {
    };

    struct CancelReject : public Message
    {
    };
}