#pragma once
#include <ostream>

namespace max::protocol_b
{
    struct NewOrderSingle
    {
        int a{};
        int b{};

        friend std::ostream &operator<<(std::ostream &os, const NewOrderSingle &msg)
        {
            os << "B NewOrderSingle a:" << msg.a << " b:" << msg.b;
            return os;
        }
    };

    struct CancelReplaceRequest
    {
        int a{};
        int b{};

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

        friend std::ostream &operator<<(std::ostream &os, const CancelRequest &msg)
        {
            os << "B CancelRequest a:" << msg.a << " b:" << msg.b;
            return os;
        }
    };

    struct ExecutionReport
    {
        int a{};
        int b{};

        friend std::ostream &operator<<(std::ostream &os, const ExecutionReport &msg)
        {
            os << "B ExecutionReport a: " << msg.a << " b:" << msg.b;
            return os;
        }
    };

    struct CancelReject
    {
        int a{};
        int b{};

        friend std::ostream &operator<<(std::ostream &os, const CancelReject &msg)
        {
            os << "B CancelReject a: " << msg.a << " b:" << msg.b;
            return os;
        }
    };
}