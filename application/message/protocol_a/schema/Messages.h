#pragma once

#include <iostream>
#include <cstdint>

namespace hyper::protocol_a::schema
{
    /* Message schema of Protocol A */

    enum class MsgType : std::uint8_t
    {
        Logon = 0,
        Logout = 1,
        Heartbeat = 2,
        NewOrderSingle = 3,
        CancelReplaceRequest = 4,
        CancelRequest = 5,
        ExecutionReport = 6,
        CancelReject = 7,
    };

#pragma pack(push, 1)
    struct Header
    {
        std::uint32_t size;
        MsgType msg_type;
        std::uint64_t seq_no;
    };
#pragma pack(pop)

#pragma pack(push, 1)
    struct Logon
    {
        Header header{.size = sizeof(Logon), .msg_type = MsgType::Logon};
        std::uint32_t a{};
        std::uint32_t b{};

        friend std::ostream &operator<<(std::ostream &os, const Logon &msg)
        {
            os << "A Logon a:" << msg.a << " b:" << msg.b;
            return os;
        }
    };
#pragma pack(pop)

#pragma pack(push, 1)
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
#pragma pack(pop)

#pragma pack(push, 1)
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
#pragma pack(pop)

#pragma pack(push, 1)
    struct NewOrderSingle
    {
        Header header{sizeof(NewOrderSingle), MsgType::NewOrderSingle};
        std::uint32_t a{};
        std::uint32_t b{};
        std::uint64_t cl_ord_id{};

        friend std::ostream &operator<<(std::ostream &os, const NewOrderSingle &msg)
        {
            os << "A NewOrderSingle a:" << msg.a
               << " b:" << msg.b
               << " cl_ord_id: " << msg.cl_ord_id;
            return os;
        }
    };
#pragma pack(pop)

#pragma pack(push, 1)
    struct CancelReplaceRequest
    {
        Header header{sizeof(CancelReplaceRequest), MsgType::CancelReplaceRequest};
        std::uint32_t a{};
        std::uint32_t b{};
        std::uint64_t cl_ord_id{};
        std::uint64_t orig_cl_ord_id{};

        friend std::ostream &operator<<(std::ostream &os, const CancelReplaceRequest &msg)
        {
            os << "B CancelReplaceRequest a:" << msg.a
               << " b:" << msg.b
               << " cl_ord_id: " << msg.cl_ord_id
               << " orig_cl_ord_id: " << msg.orig_cl_ord_id;
            return os;
        }
    };
#pragma pack(pop)

#pragma pack(push, 1)
    struct CancelRequest
    {
        Header header{sizeof(CancelRequest), MsgType::CancelRequest};
        std::uint32_t a{};
        std::uint32_t b{};
        std::uint64_t cl_ord_id{};
        std::uint64_t orig_cl_ord_id{};

        friend std::ostream &operator<<(std::ostream &os, const CancelRequest &msg)
        {
            os << "A CancelRequest a:" << msg.a
               << " b:" << msg.b
               << " cl_ord_id: " << msg.cl_ord_id
               << " orig_cl_ord_id: " << msg.orig_cl_ord_id;
            return os;
        }
    };
#pragma pack(pop)

#pragma pack(push, 1)
    struct ExecutionReport
    {
        Header header{sizeof(ExecutionReport), MsgType::ExecutionReport};
        std::uint32_t a{};
        std::uint32_t b{};
        std::uint64_t cl_ord_id{};
        std::uint64_t orig_cl_ord_id{};
        std::uint64_t ord_id{};
        std::uint64_t exec_id{};

        friend std::ostream &operator<<(std::ostream &os, const ExecutionReport &msg)
        {
            os << "A ExecutionReport a:" << msg.a
               << " b:" << msg.b
               << " cl_ord_id:" << msg.cl_ord_id
               << " orig_cl_ord_id:" << msg.orig_cl_ord_id
               << " ord_id: " << msg.ord_id
               << " exec_id:" << msg.exec_id;
            return os;
        }
    };
#pragma pack(pop)

#pragma pack(push, 1)
    struct CancelReject
    {
        Header header{sizeof(CancelReject), MsgType::CancelReject};
        std::uint32_t a{};
        std::uint32_t b{};
        std::uint64_t cl_ord_id{};
        std::uint64_t ord_id{};

        friend std::ostream &operator<<(std::ostream &os, const CancelReject &msg)
        {
            os << "A CancelReject a:" << msg.a
               << " b:" << msg.b
               << " cl_ord_id:" << msg.cl_ord_id
               << " ord_id: " << msg.ord_id;
            return os;
        }
    };
#pragma pack(pop)
}