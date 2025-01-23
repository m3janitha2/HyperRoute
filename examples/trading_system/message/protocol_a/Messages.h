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

        /* message interface required for the framework */
        [[nodiscard]] constexpr std::uint64_t cl_ord_id() const noexcept { return msg().cl_ord_id; }
        constexpr void cl_ord_id(std::uint64_t id) noexcept { msg().cl_ord_id = id; }
        std::uint64_t price() { return msg().a; }
        std::uint64_t size() { return msg().b; }
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

        /* message interface required for the framework */
        [[nodiscard]] constexpr std::uint64_t cl_ord_id() const noexcept { return msg().cl_ord_id; }
        constexpr void cl_ord_id(std::uint64_t id) noexcept { msg().cl_ord_id = id; }
        std::uint64_t orig_cl_ord_id() { return msg().orig_cl_ord_id; }
        std::uint64_t price() { return msg().a; }
        std::uint64_t size() { return msg().b; }
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

        /* message interface required for the framework */
        std::uint64_t cl_ord_id() { return msg().cl_ord_id; }
        std::uint64_t orig_cl_ord_id() { return msg().orig_cl_ord_id; }
        std::uint64_t price() { return msg().a; }
        std::uint64_t size() { return msg().b; }
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

        /* message interface required for the framework */
        [[nodiscard]] constexpr std::uint64_t cl_ord_id() const noexcept { return msg().cl_ord_id; }
        constexpr void cl_ord_id(std::uint64_t id) noexcept { msg().cl_ord_id = id; }
        [[nodiscard]] constexpr std::uint64_t orig_cl_ord_id() const noexcept { return msg().orig_cl_ord_id; }
        constexpr void orig_cl_ord_id(std::uint64_t id) noexcept { msg().orig_cl_ord_id = id; }
        [[nodiscard]] constexpr std::uint64_t price() const noexcept { return msg().a; }
        constexpr void price(std::uint64_t id) noexcept { msg().a = id; }
        [[nodiscard]] constexpr std::uint64_t size() const noexcept { return msg().b; }
        constexpr void size(std::uint64_t id) noexcept { msg().b = id; }
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

        /* message interface required for the framework */
        [[nodiscard]] constexpr std::uint64_t cl_ord_id() const noexcept { return msg().cl_ord_id; }
        constexpr void cl_ord_id(std::uint64_t id) noexcept { msg().cl_ord_id = id; }
        std::uint64_t price() { return msg().a; }
        std::uint64_t size() { return msg().b; }
    };
}