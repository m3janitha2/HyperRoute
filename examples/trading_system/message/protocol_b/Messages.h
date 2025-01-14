#pragma once

#include <examples/trading_system/message/core/BinaryMessages.h>
#include <examples/trading_system/message/protocol_b/schema/Messages.h>
#include <cstdint>
#include <iostream>

namespace hyper::protocol_b::session
{
    /* Wrap schema messages to support the trading system interface */
    /* Do not copy or allocate */

    class NewOrderSingle : public trading::binary::NewOrderSingle<schema::NewOrderSingle>
    {
    public:
        explicit constexpr NewOrderSingle(schema::NewOrderSingle &msg) noexcept
            : trading::binary::NewOrderSingle<schema::NewOrderSingle>{msg} {}
        explicit constexpr NewOrderSingle(std::string_view data) noexcept
            : trading::binary::NewOrderSingle<schema::NewOrderSingle>{data} {}

        friend std::ostream &operator<<(std::ostream &os, const NewOrderSingle &msg)
        {
            os << "NewOrderSingle: " << msg.msg();
            return os;
        }

        /* message interface required for the framework */
        [[nodiscard]] constexpr std::uint64_t cl_ord_id() const noexcept { return msg().c; }
        constexpr void cl_ord_id(std::uint64_t id) noexcept { msg().c = id; }
        [[nodiscard]] constexpr std::uint64_t price() const noexcept { return msg().a; }
        constexpr void price(std::uint64_t id) noexcept { msg().a = id; }
        [[nodiscard]] constexpr std::uint64_t size() const noexcept { return msg().b; }
        constexpr void size(std::uint64_t id) noexcept { msg().b = id; }
    };

    class CancelReplaceRequest : public trading::binary::CancelReplaceRequest<schema::CancelReplaceRequest>
    {
    public:
        explicit constexpr CancelReplaceRequest(schema::CancelReplaceRequest &msg) noexcept
            : trading::binary::CancelReplaceRequest<schema::CancelReplaceRequest>{msg} {}
        explicit constexpr CancelReplaceRequest(std::string_view data) noexcept
            : trading::binary::CancelReplaceRequest<schema::CancelReplaceRequest>{data} {}

        friend std::ostream &operator<<(std::ostream &os, const CancelReplaceRequest &msg)
        {
            os << msg.msg();
            return os;
        }

        /* message interface required for the framework */
        [[nodiscard]] constexpr std::uint64_t cl_ord_id() const noexcept { return msg().c; }
        constexpr void cl_ord_id(std::uint64_t id) noexcept { msg().c = id; }
        [[nodiscard]] constexpr std::uint64_t orig_cl_ord_id() const noexcept { return msg().o; }
        constexpr void orig_cl_ord_id(std::uint64_t id) noexcept { msg().o = id; }
        [[nodiscard]] constexpr std::uint64_t price() const noexcept { return msg().a; }
        constexpr void price(std::uint64_t id) noexcept { msg().a = id; }
        [[nodiscard]] constexpr std::uint64_t size() const noexcept { return msg().b; }
        constexpr void size(std::uint64_t id) noexcept { msg().b = id; }
    };

    class CancelRequest : public trading::binary::CancelRequest<schema::CancelRequest>
    {
    public:
        explicit constexpr CancelRequest(schema::CancelRequest &msg) noexcept
            : trading::binary::CancelRequest<schema::CancelRequest>{msg} {}
        explicit constexpr CancelRequest(std::string_view data) noexcept
            : trading::binary::CancelRequest<schema::CancelRequest>{data} {}

        friend std::ostream &operator<<(std::ostream &os, const CancelRequest &msg)
        {
            os << msg.msg();
            return os;
        }

        /* message interface required for the framework */
        [[nodiscard]] constexpr std::uint64_t cl_ord_id() const noexcept { return msg().c; }
        constexpr void cl_ord_id(std::uint64_t id) noexcept { msg().c = id; }
        [[nodiscard]] constexpr std::uint64_t orig_cl_ord_id() const noexcept { return msg().o; }
        constexpr void orig_cl_ord_id(std::uint64_t id) noexcept { msg().o = id; }
        [[nodiscard]] constexpr std::uint64_t price() const noexcept { return msg().a; }
        constexpr void price(std::uint64_t id) noexcept { msg().a = id; }
        [[nodiscard]] constexpr std::uint64_t size() const noexcept { return msg().b; }
        constexpr void size(std::uint64_t id) noexcept { msg().b = id; }
    };

    class ExecutionReport : public trading::binary::ExecutionReport<schema::ExecutionReport>
    {
    public:
        explicit constexpr ExecutionReport(schema::ExecutionReport &msg) noexcept
            : trading::binary::ExecutionReport<schema::ExecutionReport>{msg} {}
        explicit constexpr ExecutionReport(std::string_view data) noexcept
            : trading::binary::ExecutionReport<schema::ExecutionReport>{data} {}

        friend std::ostream &operator<<(std::ostream &os, const ExecutionReport &msg)
        {
            os << msg.msg();
            return os;
        }

        /* message interface required for the framework */
        [[nodiscard]] constexpr std::uint64_t cl_ord_id() const noexcept { return msg().c; }
        constexpr void cl_ord_id(std::uint64_t id) noexcept { msg().c = id; }
        [[nodiscard]] constexpr std::uint64_t orig_cl_ord_id() const noexcept { return msg().o; }
        constexpr void orig_cl_ord_id(std::uint64_t id) noexcept { msg().o = id; }
        [[nodiscard]] constexpr std::uint64_t price() const noexcept { return msg().a; }
        constexpr void price(std::uint64_t id) noexcept { msg().a = id; }
        [[nodiscard]] constexpr std::uint64_t size() const noexcept { return msg().b; }
        constexpr void size(std::uint64_t id) noexcept { msg().b = id; }
    };

    class CancelReject : public trading::binary::CancelReject<schema::CancelReject>
    {
    public:
        explicit constexpr CancelReject(schema::CancelReject &msg) noexcept
            : trading::binary::CancelReject<schema::CancelReject>{msg} {}
        explicit constexpr CancelReject(std::string_view data) noexcept
            : trading::binary::CancelReject<schema::CancelReject>{data} {}

        friend std::ostream &operator<<(std::ostream &os, const CancelReject &msg)
        {
            os << msg.msg();
            return os;
        }

        /* message interface required for the framework */
        [[nodiscard]] constexpr std::uint64_t cl_ord_id() const noexcept { return msg().c; }
        constexpr void cl_ord_id(std::uint64_t id) noexcept { msg().c = id; }
        [[nodiscard]] constexpr std::uint64_t orig_cl_ord_id() const noexcept { return msg().o; }
        constexpr void orig_cl_ord_id(std::uint64_t id) noexcept { msg().o = id; }
        [[nodiscard]] constexpr std::uint64_t price() const noexcept { return msg().a; }
        constexpr void price(std::uint64_t id) noexcept { msg().a = id; }
        [[nodiscard]] constexpr std::uint64_t size() const noexcept { return msg().b; }
        constexpr void size(std::uint64_t id) noexcept { msg().b = id; }
    };
}