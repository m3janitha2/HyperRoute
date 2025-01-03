#pragma once

#include <application/message/protocol_b/schema/Messages.h>
#include <application/message/core/Messages.h>
#include <iostream>
#include <cstdint>

namespace max::protocol_b::session
{
    class NewOrderSingle : public core::binary::NewOrderSingle<schema::NewOrderSingle>
    {
    public:
        explicit NewOrderSingle(schema::NewOrderSingle &msg)
            : core::binary::NewOrderSingle<schema::NewOrderSingle>(msg) {}

        friend std::ostream &operator<<(std::ostream &os, const NewOrderSingle &msg)
        {
            os << "NewOrderSingle: " << msg.msg_;
            return os;
        }

        /* message interface required for the freamwork */
        std::uint64_t cl_ord_id() { return msg_.a; }
    };

    class CancelReplaceRequest : public core::binary::CancelReplaceRequest<schema::CancelReplaceRequest>
    {
    public:
        explicit CancelReplaceRequest(schema::CancelReplaceRequest &msg)
            : core::binary::CancelReplaceRequest<schema::CancelReplaceRequest>(msg) {}

        friend std::ostream &operator<<(std::ostream &os, const CancelReplaceRequest &msg)
        {
            os << msg.msg_;
            return os;
        }

        /* message interface required for the freamwork */
        std::uint64_t cl_ord_id() { return msg_.a; }
        std::uint64_t orig_cl_ord_id() { return msg_.b; }
    };

    class CancelRequest : public core::binary::CancelRequest<schema::CancelRequest>
    {
    public:
        explicit CancelRequest(schema::CancelRequest &msg)
            : core::binary::CancelRequest<schema::CancelRequest>(msg) {}

        friend std::ostream &operator<<(std::ostream &os, const CancelRequest &msg)
        {
            os << msg.msg_;
            return os;
        }

        /* message interface required for the freamwork */
        std::uint64_t cl_ord_id() { return msg_.a; }
        std::uint64_t orig_cl_ord_id() { return msg_.b; }
    };

    class ExecutionReport : public core::binary::ExecutionReport<schema::ExecutionReport>
    {
    public:
        explicit ExecutionReport(schema::ExecutionReport &msg)
            : core::binary::ExecutionReport<schema::ExecutionReport>(msg) {}

        friend std::ostream &operator<<(std::ostream &os, const ExecutionReport &msg)
        {
            os << msg.msg_;
            return os;
        }

        /* message interface required for the freamwork */
        std::uint64_t cl_ord_id() { return msg_.a; }
    };

    class CancelReject : public core::binary::CancelReject<schema::CancelReject>
    {
    public:
        explicit CancelReject(schema::CancelReject &msg)
            : core::binary::CancelReject<schema::CancelReject>(msg) {}

        friend std::ostream &operator<<(std::ostream &os, const CancelReject &msg)
        {
            os << msg.msg_;
            return os;
        }

        /* message interface required for the freamwork */
        std::uint64_t cl_ord_id() { return msg_.a; }
    };
}