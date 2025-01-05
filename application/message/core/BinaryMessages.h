#pragma once

#include <application/message/core/Types.h>
#include <application/message/core/Messages.h>
#include <ostream>
#include <string_view>
#include <chrono>

namespace max::message::binary
{
    enum MessageAllocationPolicy
    {
        AcquireByReference,
        AllocateOnHeap
    };

    template <typename Msg, MessageAllocationPolicy = MessageAllocationPolicy::AcquireByReference>
    class BinaryMessage
    {
    public:
        explicit constexpr BinaryMessage(Msg &msg) noexcept
            : msg_{msg} {}
        explicit constexpr BinaryMessage(std::string_view data) noexcept
            : msg_{*(reinterpret_cast<Msg *>(const_cast<char*>(data.data())))} {}

        [[nodiscard]] constexpr const Msg &msg() const noexcept { return msg_; }
        [[nodiscard]] constexpr Msg &msg() noexcept { return msg_; }
        [[nodiscard]] constexpr std::string_view data() const noexcept
        {
            std::string_view data{reinterpret_cast<char *>(&msg_), sizeof(Msg)};
            return data;
        }

    private:
        Msg &msg_;
    };

    template <typename Msg>
    struct NewOrderSingle : public BinaryMessage<Msg>, core::NewOrderSingle
    {
        explicit constexpr NewOrderSingle(Msg &msg) noexcept
            : BinaryMessage<Msg>{msg} {}

        explicit constexpr NewOrderSingle(std::string_view data) noexcept
            : BinaryMessage<Msg>{data} {}
    };

    template <typename Msg>
    struct CancelReplaceRequest : public BinaryMessage<Msg>, core::CancelReplaceRequest
    {
        explicit constexpr CancelReplaceRequest(Msg &msg) noexcept
            : BinaryMessage<Msg>{msg} {}

        explicit constexpr CancelReplaceRequest(std::string_view data) noexcept
            : BinaryMessage<Msg>{data} {}
    };

    template <typename Msg>
    struct CancelRequest : public BinaryMessage<Msg>, core::CancelRequest
    {
        explicit constexpr CancelRequest(Msg &msg) noexcept
            : BinaryMessage<Msg>{msg} {}
            
        explicit constexpr CancelRequest(std::string_view data) noexcept
            : BinaryMessage<Msg>{data} {}
    };

    template <typename Msg>
    struct ExecutionReport : public BinaryMessage<Msg>, core::ExecutionReport
    {
        explicit constexpr ExecutionReport(Msg &msg) noexcept
            : BinaryMessage<Msg>{msg} {}
            
        explicit constexpr ExecutionReport(std::string_view data) noexcept
            : BinaryMessage<Msg>{data} {}
    };

    template <typename Msg>
    struct CancelReject : public BinaryMessage<Msg>, core::CancelReject
    {
        explicit constexpr CancelReject(Msg &msg) noexcept
            : BinaryMessage<Msg>{msg} {}
            
        explicit constexpr CancelReject(std::string_view data) noexcept
            : BinaryMessage<Msg>{data} {}
    };
}