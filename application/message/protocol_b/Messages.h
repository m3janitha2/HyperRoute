#pragma once

#include <application/message/protocol_b/schema/Messages.h>
#include <application/message/core/BinaryMessages.h>
#include <iostream>
#include <cstdint>

namespace max::protocol_b::session
{
    class NewOrderSingle : public message::binary::NewOrderSingle<schema::NewOrderSingle>
    {
    public:
        explicit NewOrderSingle(schema::NewOrderSingle &msg)
            : message::binary::NewOrderSingle<schema::NewOrderSingle>(msg) {}

        friend std::ostream &operator<<(std::ostream &os, const NewOrderSingle &msg)
        {
            os << "NewOrderSingle: " << msg.msg();
            return os;
        }

        /* message interface required for the freamwork */
        std::uint64_t cl_ord_id() { return msg().a; }
    };

    class CancelReplaceRequest : public message::binary::CancelReplaceRequest<schema::CancelReplaceRequest>
    {
    public:
        explicit CancelReplaceRequest(schema::CancelReplaceRequest &msg)
            : message::binary::CancelReplaceRequest<schema::CancelReplaceRequest>(msg) {}

        friend std::ostream &operator<<(std::ostream &os, const CancelReplaceRequest &msg)
        {
            os << msg.msg();
            return os;
        }

        /* message interface required for the freamwork */
        std::uint64_t cl_ord_id() { return msg().a; }
        std::uint64_t orig_cl_ord_id() { return msg().b; }
    };

    class CancelRequest : public message::binary::CancelRequest<schema::CancelRequest>
    {
    public:
        explicit CancelRequest(schema::CancelRequest &msg)
            : message::binary::CancelRequest<schema::CancelRequest>(msg) {}

        friend std::ostream &operator<<(std::ostream &os, const CancelRequest &msg)
        {
            os << msg.msg();
            return os;
        }

        /* message interface required for the freamwork */
        std::uint64_t cl_ord_id() { return msg().a; }
        std::uint64_t orig_cl_ord_id() { return msg().b; }
    };

    class ExecutionReport : public message::binary::ExecutionReport<schema::ExecutionReport>
    {
    public:
        explicit ExecutionReport(schema::ExecutionReport &msg)
            : message::binary::ExecutionReport<schema::ExecutionReport>(msg) {}

        friend std::ostream &operator<<(std::ostream &os, const ExecutionReport &msg)
        {
            os << msg.msg();
            return os;
        }

        /* message interface required for the freamwork */
        std::uint64_t cl_ord_id() { return msg().a; }
    };

    class CancelReject : public message::binary::CancelReject<schema::CancelReject>
    {
    public:
        explicit CancelReject(schema::CancelReject &msg)
            : message::binary::CancelReject<schema::CancelReject>(msg) {}

        friend std::ostream &operator<<(std::ostream &os, const CancelReject &msg)
        {
            os << msg.msg();
            return os;
        }

        /* message interface required for the freamwork */
        std::uint64_t cl_ord_id() { return msg().a; }
    };
}