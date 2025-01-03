#pragma once

#include <ostream>
#include <string_view>

namespace max::core::binary
{
    template <typename Msg>
    struct BinaryMessage
    {
        explicit BinaryMessage(Msg &msg)
            : msg_{msg}{}

        const Msg &msg() const { return msg_; }
        Msg &msg() { return msg_; }
        std::string_view data()
        {
            std::string_view data{reinterpret_cast<char *>(msg_), sizeof(msg)};
            return data;
        }

        Msg &msg_;
    };

    template <typename Msg>
    struct NewOrderSingle : public BinaryMessage<Msg>
    {
        explicit NewOrderSingle(Msg &msg)
            : BinaryMessage<Msg>(msg) {}

        friend std::ostream &operator<<(std::ostream &os, const Msg &msg)
        {
            os << "NewOrderSingle: " << msg;
            return os;
        }
    };

    template <typename Msg>
    struct CancelReplaceRequest : public BinaryMessage<Msg>
    {
        explicit CancelReplaceRequest(Msg &msg)
            : BinaryMessage<Msg>(msg) {}

        friend std::ostream &operator<<(std::ostream &os, const Msg &msg)
        {
            os << "CancelReplaceRequest: " << msg;
            return os;
        }
    };

    template <typename Msg>
    struct CancelRequest : public BinaryMessage<Msg>
    {
        explicit CancelRequest(Msg &msg)
            : BinaryMessage<Msg>(msg) {}

        friend std::ostream &operator<<(std::ostream &os, const Msg &msg)
        {
            os << "CancelRequest: " << msg;
            return os;
        }
    };

    template <typename Msg>
    struct ExecutionReport : public BinaryMessage<Msg>
    {
        explicit ExecutionReport(Msg &msg)
            : BinaryMessage<Msg>(msg) {}

        friend std::ostream &operator<<(std::ostream &os, const Msg &msg)
        {
            os << "ExecutionReport: " << msg;
            return os;
        }
    };

    template <typename Msg>
    struct CancelReject : public BinaryMessage<Msg>
    {
        explicit CancelReject(Msg &msg)
            : BinaryMessage<Msg>(msg) {}
        friend std::ostream &operator<<(std::ostream &os, const Msg &msg)
        {
            os << "CancelReject: " << msg;
            return os;
        }
    };
}