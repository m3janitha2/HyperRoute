#pragma once

#include <application/message/core/BinaryMessages.h>
#include <application/message/protocol_b/schema/Messages.h>
#include <cstdint>
#include <iostream>

namespace hyper::protocol_b::session
{
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
        std::uint64_t cl_ord_id() { return msg().a; }
        std::uint64_t price() { return msg().a; }
        std::uint64_t size() { return msg().b; }
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
        std::uint64_t cl_ord_id() { return msg().a; }
        std::uint64_t orig_cl_ord_id() { return msg().b; }
        std::uint64_t price() { return msg().a; }
        std::uint64_t size() { return msg().b; }
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
        std::uint64_t cl_ord_id() { return msg().a; }
        std::uint64_t orig_cl_ord_id() { return msg().b; }
        std::uint64_t price() { return msg().a; }
        std::uint64_t size() { return msg().b; }
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
        std::uint64_t cl_ord_id() { return msg().a; }
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
        std::uint64_t cl_ord_id() { return msg().a; }
        std::uint64_t price() { return msg().a; }
        std::uint64_t size() { return msg().b; }
    };
}