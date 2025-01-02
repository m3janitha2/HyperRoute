#pragma once
#include <ostream>

namespace max::core
{
    template <typename Msg>
    struct NewOrderSingle
    {
        Msg &msg_;
        
        friend std::ostream &operator<<(std::ostream &os, const NewOrderSingle &msg)
        {
            os << "NewOrderSingle: " << msg;
            return os;
        }
    };

    struct CancelReplaceRequest
    {
        int a{};
        int b{};
        int cl_ord_id{};
        int orig_cl_ord_id{};

        friend std::ostream &operator<<(std::ostream &os, const CancelReplaceRequest &msg)
        {
            os << "B CancelReplaceRequest a:" << msg.a << " b:" << msg.b;
            return os;
        }
    };

    struct CancelRequest
    {
        int a{};
        int b{};
        int cl_ord_id{};
        int orig_cl_ord_id{};

        friend std::ostream &operator<<(std::ostream &os, const CancelRequest &msg)
        {
            os << "A CancelRequest a:" << msg.a << " b:" << msg.b;
            return os;
        }
    };

    struct ExecutionReport
    {
        int a{};
        int b{};

        friend std::ostream &operator<<(std::ostream &os, const ExecutionReport &msg)
        {
            os << "A ExecutionReport a:" << msg.a << " b:" << msg.b;
            return os;
        }
    };

    struct CancelReject
    {
        int a{};
        int b{};

        friend std::ostream &operator<<(std::ostream &os, const CancelReject &msg)
        {
            os << "A CancelReject a:" << msg.a << " b:" << msg.b;
            return os;
        }
    };
}