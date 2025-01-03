#pragma once

#include <iostream>
#include <cstdint>

namespace max::protocol_b::schema
{
    enum class MsgType : std::uint32_t
    {
        Logon = 10,
        Logout = 11,
        Heartbeat = 12,
        NewOrderSingle = 13,
        CancelReplaceRequest = 14,
        CancelRequest = 15,
        ExecutionReport = 16,
        CancelReject = 17,
    };

    struct Header
    {
        std::uint32_t size;
        MsgType msg_type;
        std::uint8_t version{1};
    };

    struct Logon
    {
        Header header{sizeof(Logon), MsgType::Logon};
        std::uint32_t a{};
        std::uint32_t b{};

        friend std::ostream &operator<<(std::ostream &os, const Logon &msg)
        {
            os << "A Logon a:" << msg.a << " b:" << msg.b;
            return os;
        }
    };

    struct Logout
    {
        Header header{sizeof(Logout), MsgType::Logout};
        std::uint32_t a{};
        std::uint32_t b{};

        friend std::ostream &operator<<(std::ostream &os, const Logout &msg)
        {
            os << "A Logout a:" << msg.a << " b:" << msg.b;
            return os;
        }
    };

    struct Heartbeat
    {
        Header header{sizeof(Heartbeat), MsgType::Heartbeat};
        std::uint32_t a{};
        std::uint32_t b{};

        friend std::ostream &operator<<(std::ostream &os, const Heartbeat &msg)
        {
            os << "A Heartbeat a:" << msg.a << " b:" << msg.b;
            return os;
        }
    };

    struct NewOrderSingle
    {
        Header header{sizeof(NewOrderSingle), MsgType::NewOrderSingle};
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
        Header header{sizeof(CancelReplaceRequest), MsgType::CancelReplaceRequest};
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
        Header header{sizeof(CancelRequest), MsgType::CancelRequest};
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
        Header header{sizeof(ExecutionReport), MsgType::ExecutionReport};
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
        Header header{sizeof(CancelReject), MsgType::CancelReject};
        int a{};
        int b{};

        friend std::ostream &operator<<(std::ostream &os, const CancelReject &msg)
        {
            os << "B CancelReject a: " << msg.a << " b:" << msg.b;
            return os;
        }
    };
}