#pragma once

#include <framework/message/BinaryMessage.h>
#include <examples/trading_system/message/core/Messages.h>
#include <examples/trading_system/message/protocol_a/schema/Messages.h>
#include <cstdint>
#include <iostream>

namespace hyper::protocol_a::session
{
    using Timestamp = framework::Timestamp;
    using TimestampClock = framework::TimestampClock;

    /* Wrap schema messages to support the trading system interface */
    /* Do not copy or allocate */

    class NewOrderSingle : public framework::BinaryMessage<schema::NewOrderSingle>,
                           public trading::NewOrderSingle
    {
    public:
        explicit constexpr NewOrderSingle(schema::NewOrderSingle &msg,
                                          Timestamp in_timestamp = TimestampClock::now()) noexcept
            : framework::BinaryMessage<schema::NewOrderSingle>{msg, in_timestamp} {}

        explicit constexpr NewOrderSingle(std::string_view data,
                                          Timestamp in_timestamp = TimestampClock::now()) noexcept
            : framework::BinaryMessage<schema::NewOrderSingle>{data, in_timestamp} {}

        friend std::ostream &operator<<(std::ostream &os, const NewOrderSingle &msg)
        {
            os << msg.msg();
            return os;
        }

        /* Framework-required message interface */
        [[nodiscard]] constexpr std::uint64_t cl_ord_id() const noexcept { return msg().cl_ord_id; }
        constexpr void cl_ord_id(std::uint64_t id) noexcept { msg().cl_ord_id = id; }

        [[nodiscard]] constexpr std::uint64_t price() const noexcept { return msg().a; }
        constexpr void price(std::uint64_t p) noexcept { msg().a = p; }

        [[nodiscard]] constexpr std::uint64_t size() const noexcept { return msg().b; }
        constexpr void size(std::uint64_t s) noexcept { msg().b = s; }
    };

    class CancelReplaceRequest : public framework::BinaryMessage<schema::CancelReplaceRequest>,
                                 public trading::CancelReplaceRequest
    {
    public:
        explicit constexpr CancelReplaceRequest(schema::CancelReplaceRequest &msg,
                                                Timestamp in_timestamp = TimestampClock::now()) noexcept
            : framework::BinaryMessage<schema::CancelReplaceRequest>{msg, in_timestamp} {}

        explicit constexpr CancelReplaceRequest(std::string_view data,
                                                Timestamp in_timestamp = TimestampClock::now()) noexcept
            : framework::BinaryMessage<schema::CancelReplaceRequest>{data, in_timestamp} {}

        friend std::ostream &operator<<(std::ostream &os, const CancelReplaceRequest &msg)
        {
            os << msg.msg();
            return os;
        }

        /* Framework-required message interface */
        [[nodiscard]] constexpr std::uint64_t cl_ord_id() const noexcept { return msg().cl_ord_id; }
        constexpr void cl_ord_id(std::uint64_t id) noexcept { msg().cl_ord_id = id; }

        [[nodiscard]] constexpr std::uint64_t orig_cl_ord_id() const noexcept { return msg().orig_cl_ord_id; }
        constexpr void orig_cl_ord_id(std::uint64_t id) noexcept { msg().orig_cl_ord_id = id; }

        [[nodiscard]] constexpr std::uint64_t price() const noexcept { return msg().a; }
        constexpr void price(std::uint64_t p) noexcept { msg().a = p; }

        [[nodiscard]] constexpr std::uint64_t size() const noexcept { return msg().b; }
        constexpr void size(std::uint64_t s) noexcept { msg().b = s; }
    };

    class CancelRequest : public framework::BinaryMessage<schema::CancelRequest>,
                          public trading::CancelRequest
    {
    public:
        explicit constexpr CancelRequest(schema::CancelRequest &msg,
                                         Timestamp in_timestamp = TimestampClock::now()) noexcept
            : framework::BinaryMessage<schema::CancelRequest>{msg, in_timestamp} {}

        explicit constexpr CancelRequest(std::string_view data,
                                         Timestamp in_timestamp = TimestampClock::now()) noexcept
            : framework::BinaryMessage<schema::CancelRequest>{data, in_timestamp} {}

        friend std::ostream &operator<<(std::ostream &os, const CancelRequest &msg)
        {
            os << msg.msg();
            return os;
        }

        /* Framework-required message interface */
        [[nodiscard]] constexpr std::uint64_t cl_ord_id() const noexcept { return msg().cl_ord_id; }
        constexpr void cl_ord_id(std::uint64_t id) noexcept { msg().cl_ord_id = id; }

        [[nodiscard]] constexpr std::uint64_t orig_cl_ord_id() const noexcept { return msg().orig_cl_ord_id; }
        constexpr void orig_cl_ord_id(std::uint64_t id) noexcept { msg().orig_cl_ord_id = id; }

        [[nodiscard]] constexpr std::uint64_t price() const noexcept { return msg().a; }
        constexpr void price(std::uint64_t p) noexcept { msg().a = p; }

        [[nodiscard]] constexpr std::uint64_t size() const noexcept { return msg().b; }
        constexpr void size(std::uint64_t s) noexcept { msg().b = s; }
    };

    class ExecutionReport : public framework::BinaryMessage<schema::ExecutionReport>,
                            public trading::ExecutionReport
    {
    public:
        explicit constexpr ExecutionReport(schema::ExecutionReport &msg,
                                           Timestamp in_timestamp = TimestampClock::now()) noexcept
            : framework::BinaryMessage<schema::ExecutionReport>{msg, in_timestamp} {}

        explicit constexpr ExecutionReport(std::string_view data,
                                           Timestamp in_timestamp = TimestampClock::now()) noexcept
            : framework::BinaryMessage<schema::ExecutionReport>{data, in_timestamp} {}

        friend std::ostream &operator<<(std::ostream &os, const ExecutionReport &msg)
        {
            os << msg.msg();
            return os;
        }

        /* Framework-required message interface */
        [[nodiscard]] constexpr std::uint64_t cl_ord_id() const noexcept { return msg().cl_ord_id; }
        constexpr void cl_ord_id(std::uint64_t id) noexcept { msg().cl_ord_id = id; }

        [[nodiscard]] constexpr std::uint64_t orig_cl_ord_id() const noexcept { return msg().orig_cl_ord_id; }
        constexpr void orig_cl_ord_id(std::uint64_t id) noexcept { msg().orig_cl_ord_id = id; }

        [[nodiscard]] constexpr std::uint64_t price() const noexcept { return msg().a; }
        constexpr void price(std::uint64_t p) noexcept { msg().a = p; }

        [[nodiscard]] constexpr std::uint64_t size() const noexcept { return msg().b; }
        constexpr void size(std::uint64_t s) noexcept { msg().b = s; }
    };

    class CancelReject : public framework::BinaryMessage<schema::CancelReject>,
                         public trading::CancelReject
    {
    public:
        explicit constexpr CancelReject(schema::CancelReject &msg,
                                        Timestamp in_timestamp = TimestampClock::now()) noexcept
            : framework::BinaryMessage<schema::CancelReject>{msg, in_timestamp} {}

        explicit constexpr CancelReject(std::string_view data,
                                        Timestamp in_timestamp = TimestampClock::now()) noexcept
            : framework::BinaryMessage<schema::CancelReject>{data, in_timestamp} {}

        friend std::ostream &operator<<(std::ostream &os, const CancelReject &msg)
        {
            os << msg.msg();
            return os;
        }

        /* Framework-required message interface */
        [[nodiscard]] constexpr std::uint64_t cl_ord_id() const noexcept { return msg().cl_ord_id; }
        constexpr void cl_ord_id(std::uint64_t id) noexcept { msg().cl_ord_id = id; }

        [[nodiscard]] constexpr std::uint64_t price() const noexcept { return msg().a; }
        constexpr void price(std::uint64_t p) noexcept { msg().a = p; }

        [[nodiscard]] constexpr std::uint64_t size() const noexcept { return msg().b; }
        constexpr void size(std::uint64_t s) noexcept { msg().b = s; }
    };
}

namespace hyper::protocol_a::protocol
{
    using Timestamp = framework::Timestamp;
    using TimestampClock = framework::TimestampClock;

    class Logon : public framework::BinaryMessage<schema::Logon>
    {
    public:
        explicit constexpr Logon(schema::Logon &msg,
                                 Timestamp in_timestamp = TimestampClock::now()) noexcept
            : framework::BinaryMessage<schema::Logon>{msg, in_timestamp} {}

        explicit constexpr Logon(std::string_view data,
                                 Timestamp in_timestamp = TimestampClock::now()) noexcept
            : framework::BinaryMessage<schema::Logon>{data, in_timestamp} {}

        friend std::ostream &operator<<(std::ostream &os, const Logon &msg)
        {
            os << msg.msg();
            return os;
        }
    };

    class Logout : public framework::BinaryMessage<schema::Logout>
    {
    public:
        explicit constexpr Logout(schema::Logout &msg,
                                  Timestamp in_timestamp = TimestampClock::now()) noexcept
            : framework::BinaryMessage<schema::Logout>{msg, in_timestamp} {}

        explicit constexpr Logout(std::string_view data,
                                  Timestamp in_timestamp = TimestampClock::now()) noexcept
            : framework::BinaryMessage<schema::Logout>{data, in_timestamp} {}

        friend std::ostream &operator<<(std::ostream &os, const Logout &msg)
        {
            os << msg.msg();
            return os;
        }
    };

    class Heartbeat : public framework::BinaryMessage<schema::Heartbeat>
    {
    public:
        explicit constexpr Heartbeat(schema::Heartbeat &msg,
                                     Timestamp in_timestamp = TimestampClock::now()) noexcept
            : framework::BinaryMessage<schema::Heartbeat>{msg, in_timestamp} {}

        explicit constexpr Heartbeat(std::string_view data,
                                     Timestamp in_timestamp = TimestampClock::now()) noexcept
            : framework::BinaryMessage<schema::Heartbeat>{data, in_timestamp} {}

        friend std::ostream &operator<<(std::ostream &os, const Heartbeat &msg)
        {
            os << msg.msg();
            return os;
        }
    };
}