#pragma once

#include <application/message/core/Types.h>
#include <application/message/core/Messages.h>
#include <ostream>
#include <string_view>
#include <chrono>

namespace max::message::binary
{
    namespace testx
    {
        // enum AllocationPolicy
        // {
        //     AcquireByReference,
        //     AllocateOnHeap
        // };

        // struct AllocationPolicyAcquireByReference
        // {
        // };
        // struct AllocationPolicyAllocateOnHeap
        // {
        // };

        // template <typename Msg, typename AllocationPolicy>
        // class BinaryMessage;

        // template <typename Msg>
        // class BinaryMessage<Msg, AllocationPolicyAcquireByReference> : public BinaryMessageAcquireByReference
        // {
        //     explicit constexpr BinaryMessage(Msg &msg) noexcept
        //         : BinaryMessageAcquireByReference{msg} {}
        //     explicit constexpr BinaryMessage(std::string_view data) noexcept
        //         : BinaryMessageAcquireByReference{data} {}
        // };

        // template <typename Msg>
        // class BinaryMessage<Msg, AllocationPolicyAcquireByReference> : public BinaryMessageAllocateOnHeap
        // {
        // };

        // template <typename Msg>
        // class BinaryMessageAcquireByReference
        // {
        // public:
        //     explicit constexpr BinaryMessageAcquireByReference(Msg &msg) noexcept
        //         : msg_{msg} {}
        //     explicit constexpr BinaryMessageAcquireByReference(std::string_view data) noexcept
        //         : msg_{*(reinterpret_cast<Msg *>(const_cast<char *>(data.data())))} {}

        //     [[nodiscard]] constexpr const Msg &msg() const noexcept { return msg_; }
        //     [[nodiscard]] constexpr Msg &msg() noexcept { return msg_; }
        //     [[nodiscard]] constexpr std::string_view data() const noexcept
        //     {
        //         std::string_view data{reinterpret_cast<char *>(&msg_), sizeof(Msg)};
        //         return data;
        //     }

        // private:
        //     Msg &msg_;
        // };

        // template <typename Msg, AllocationPolicyAcquireByReference>
        // class BinaryMessageAllocateOnHeap
        // {
        // public:
        //     [[nodiscard]] constexpr const Msg &msg() const noexcept { return msg_; }
        //     [[nodiscard]] constexpr Msg &msg() noexcept { return msg_; }
        //     [[nodiscard]] constexpr std::string_view data() const noexcept
        //     {
        //         std::string_view data{reinterpret_cast<char *>(&msg_), sizeof(Msg)};
        //         return data;
        //     }

        // private:
        //     Msg msg_{};
        // };
    }

    enum MessageAllocationPolicy
    {
        AcquireByReference,
        AllocateOnHeap
    };

    template <typename Msg, MessageAllocationPolicy = MessageAllocationPolicy::AcquireByReference>
    class BinaryMessage
    {
    public:
        BinaryMessage() = default;
        explicit constexpr BinaryMessage(Msg &msg) noexcept
            : msg_{&msg} {}
        explicit constexpr BinaryMessage(std::string_view data) noexcept
            : msg_{(reinterpret_cast<Msg *>(const_cast<char *>(data.data())))} {}

        [[nodiscard]] constexpr const Msg &msg() const noexcept { return *msg_; }
        [[nodiscard]] constexpr Msg &msg() noexcept { return *msg_; }
        [[nodiscard]] constexpr std::string_view data() const noexcept
        {
            std::string_view data{reinterpret_cast<char *>(msg_), sizeof(Msg)};
            return data;
        }

    private:
        Msg *msg_{nullptr};
    };

    template <typename Msg>
    struct NewOrderSingle : public BinaryMessage<Msg>, core::NewOrderSingle
    {
        NewOrderSingle() = default;
        explicit constexpr NewOrderSingle(Msg &msg) noexcept
            : BinaryMessage<Msg>{msg} {}
        explicit constexpr NewOrderSingle(std::string_view data) noexcept
            : BinaryMessage<Msg>{data} {}
    };

    template <typename Msg>
    struct CancelReplaceRequest : public BinaryMessage<Msg>, core::CancelReplaceRequest
    {
        CancelReplaceRequest() = default;
        explicit constexpr CancelReplaceRequest(Msg &msg) noexcept
            : BinaryMessage<Msg>{msg} {}
        explicit constexpr CancelReplaceRequest(std::string_view data) noexcept
            : BinaryMessage<Msg>{data} {}
    };

    template <typename Msg>
    struct CancelRequest : public BinaryMessage<Msg>, core::CancelRequest
    {
        CancelRequest() = default;
        explicit constexpr CancelRequest(Msg &msg) noexcept
            : BinaryMessage<Msg>{msg} {}
        explicit constexpr CancelRequest(std::string_view data) noexcept
            : BinaryMessage<Msg>{data} {}
    };

    template <typename Msg>
    struct ExecutionReport : public BinaryMessage<Msg>, core::ExecutionReport
    {
        ExecutionReport() = default;
        explicit constexpr ExecutionReport(Msg &msg) noexcept
            : BinaryMessage<Msg>{msg} {}
        explicit constexpr ExecutionReport(std::string_view data) noexcept
            : BinaryMessage<Msg>{data} {}
    };

    template <typename Msg>
    struct CancelReject : public BinaryMessage<Msg>, core::CancelReject
    {
        CancelReject() = default;
        explicit constexpr CancelReject(Msg &msg) noexcept
            : BinaryMessage<Msg>{msg} {}
        explicit constexpr CancelReject(std::string_view data) noexcept
            : BinaryMessage<Msg>{data} {}
    };
}