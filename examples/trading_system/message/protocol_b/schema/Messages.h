#pragma once

#include <iostream>
#include <cstdint>

namespace hyper::protocol_b::schema
{
    /* Message schema for Protocol B */
    /* This is a primitive, FIX-like binary schema to demonstrate usage */
    /* 60-70% of the schema is duplicated from Protocol A for implementation simplicity */

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
        std::uint64_t seq_no{0};

        friend std::ostream &operator<<(std::ostream &os, const Header &header)
        {
            os << "ProtocolB size:" << header.size
               << " msg_type:" << header.size
               << " version:" << static_cast<int>(header.version)
               << " seq_no:" << header.seq_no;
            return os;
        }
    };

    struct Logon
    {
        Header header{sizeof(Logon), MsgType::Logon};
        std::uint32_t a{};
        std::uint32_t b{};

        friend std::ostream &operator<<(std::ostream &os, const Logon &msg)
        {
            os << msg.header
               << " Logon a:" << msg.a << " b:" << msg.b;
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
            os << msg.header
               << " Logout a:" << msg.a << " b:" << msg.b;
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
            os << msg.header
               << " Heartbeat a:" << msg.a << " b:" << msg.b;
            return os;
        }
    };

    struct NewOrderSingle
    {
        Header header{sizeof(NewOrderSingle), MsgType::NewOrderSingle};
        int a{};
        int b{};
        int c{};

        friend std::ostream &operator<<(std::ostream &os, const NewOrderSingle &msg)
        {
            os << msg.header
               << " NewOrderSingle a:" << msg.a
               << " b:" << msg.b
               << " C:" << msg.c;
            return os;
        }
    };

    struct CancelReplaceRequest
    {
        Header header{sizeof(CancelReplaceRequest), MsgType::CancelReplaceRequest};
        int a{};
        int b{};
        int c{};
        int o{};

        friend std::ostream &operator<<(std::ostream &os, const CancelReplaceRequest &msg)
        {
            os << msg.header
               << " CancelReplaceRequest a:" << msg.a << " b:" << msg.b;
            return os;
        }
    };

    struct CancelRequest
    {
        Header header{sizeof(CancelRequest), MsgType::CancelRequest};
        int a{};
        int b{};
        int o{};
        int c{};

        friend std::ostream &operator<<(std::ostream &os, const CancelRequest &msg)
        {
            os << msg.header
               << " CancelRequest a:" << msg.a << " b:" << msg.b;
            return os;
        }
    };

    struct ExecutionReport
    {
        Header header{sizeof(ExecutionReport), MsgType::ExecutionReport};
        int a{};
        int b{};
        int c{};
        int o{};
        int e{};

        friend std::ostream &operator<<(std::ostream &os, const ExecutionReport &msg)
        {
            os << msg.header
               << " ExecutionReport a: " << msg.a << " b:" << msg.b;
            return os;
        }
    };

    struct CancelReject
    {
        Header header{sizeof(CancelReject), MsgType::CancelReject};
        int a{};
        int b{};
        int c{};
        int o{};

        friend std::ostream &operator<<(std::ostream &os, const CancelReject &msg)
        {
            os << msg.header
               << " CancelReject a: " << msg.a << " b:" << msg.b;
            return os;
        }
    };
}