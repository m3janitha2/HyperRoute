#pragma once

#include <framework/message/BinaryMessages.h>
#include <application/message/protocol_a/schema/Messages.h>
#include <iostream>
#include <cstdint>

namespace max::protocol_a::session
{
    class NewOrderSingle : public framework::message::binary::NewOrderSingle<schema::NewOrderSingle>
    {
    public:
        explicit constexpr NewOrderSingle(schema::NewOrderSingle &msg) noexcept
            : framework::message::binary::NewOrderSingle<schema::NewOrderSingle>{msg} {}
        explicit constexpr NewOrderSingle(std::string_view data) noexcept
            : framework::message::binary::NewOrderSingle<schema::NewOrderSingle>{data} {}

        friend std::ostream &operator<<(std::ostream &os, const NewOrderSingle &msg)
        {
            os << "NewOrderSingle: " << msg.msg();
            return os;
        }

        /* message interface required for the framework */
        std::uint64_t cl_ord_id() { return msg().cl_ord_id; }
    };

    class CancelReplaceRequest : public framework::message::binary::CancelReplaceRequest<schema::CancelReplaceRequest>
    {
    public:
        explicit constexpr CancelReplaceRequest(schema::CancelReplaceRequest &msg) noexcept
            : framework::message::binary::CancelReplaceRequest<schema::CancelReplaceRequest>{msg} {}
        explicit constexpr CancelReplaceRequest(std::string_view data) noexcept
            : framework::message::binary::CancelReplaceRequest<schema::CancelReplaceRequest>{data} {}

        friend std::ostream &operator<<(std::ostream &os, const CancelReplaceRequest &msg)
        {
            os << msg.msg();
            return os;
        }

        /* message interface required for the framework */
        std::uint64_t cl_ord_id() { return msg().cl_ord_id; }
        std::uint64_t orig_cl_ord_id() { return msg().orig_cl_ord_id; }
    };

    class CancelRequest : public framework::message::binary::CancelRequest<schema::CancelRequest>
    {
    public:
        explicit constexpr CancelRequest(schema::CancelRequest &msg) noexcept
            : framework::message::binary::CancelRequest<schema::CancelRequest>{msg} {}
        explicit constexpr CancelRequest(std::string_view data) noexcept
            : framework::message::binary::CancelRequest<schema::CancelRequest>{data} {}

        friend std::ostream &operator<<(std::ostream &os, const CancelRequest &msg)
        {
            os << msg.msg();
            return os;
        }

        /* message interface required for the framework */
        std::uint64_t cl_ord_id() { return msg().cl_ord_id; }
        std::uint64_t orig_cl_ord_id() { return msg().orig_cl_ord_id; }
    };

    class ExecutionReport : public framework::message::binary::ExecutionReport<schema::ExecutionReport>
    {
    public:
        explicit constexpr ExecutionReport(schema::ExecutionReport &msg) noexcept
            : framework::message::binary::ExecutionReport<schema::ExecutionReport>{msg} {}
        explicit constexpr ExecutionReport(std::string_view data) noexcept
            : framework::message::binary::ExecutionReport<schema::ExecutionReport>{data} {}

        friend std::ostream &operator<<(std::ostream &os, const ExecutionReport &msg)
        {
            os << msg.msg();
            return os;
        }

        /* message interface required for the framework */
        std::uint64_t cl_ord_id() { return msg().cl_ord_id; }
    };

    class CancelReject : public framework::message::binary::CancelReject<schema::CancelReject>
    {
    public:
        explicit constexpr CancelReject(schema::CancelReject &msg) noexcept
            : framework::message::binary::CancelReject<schema::CancelReject>{msg} {}
        explicit constexpr CancelReject(std::string_view data) noexcept
            : framework::message::binary::CancelReject<schema::CancelReject>{data} {}

        friend std::ostream &operator<<(std::ostream &os, const CancelReject &msg)
        {
            os << msg.msg();
            return os;
        }

        /* message interface required for the framework */
        std::uint64_t cl_ord_id() { return msg().cl_ord_id; }
    };
}