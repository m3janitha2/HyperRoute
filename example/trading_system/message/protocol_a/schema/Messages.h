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
        Header header{.size = sizeof(Logon),
                      .msg_type = MsgType::Logon,
                      .seq_no = 0};
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
        Header header{.size = sizeof(Logon),
                      .msg_type = MsgType::Logon,
                      .seq_no = 0};
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
        Header header{.size = sizeof(Logon),
                      .msg_type = MsgType::Logon,
                      .seq_no = 0};
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
        Header header{.size = sizeof(NewOrderSingle),
                      .msg_type = MsgType::NewOrderSingle,
                      .seq_no = 0};
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
        Header header{.size = sizeof(CancelReplaceRequest),
                      .msg_type = MsgType::CancelReplaceRequest,
                      .seq_no = 0};
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
        Header header{.size = sizeof(CancelRequest),
                      .msg_type = MsgType::CancelRequest,
                      .seq_no = 0};
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
        Header header{.size = sizeof(ExecutionReport),
                      .msg_type = MsgType::ExecutionReport,
                      .seq_no = 0};
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
        Header header{.size = sizeof(CancelReject),
                      .msg_type = MsgType::CancelReject,
                      .seq_no = 0};
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