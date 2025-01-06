#pragma once

#include <framework/utility/Types.h>
#include <framework/message/Messages.h>
#include <ostream>
#include <string_view>
#include <chrono>
#include <memory>

namespace max::framework::message::binary
{
    namespace testx
    {
        struct AllocationPolicyAcquireByReference
        {
        };

        struct AllocationPolicyAllocateOnHeap
        {
        };

        template <typename Msg, typename AllocationPolicy = AllocationPolicyAcquireByReference>
        class BinaryMessage
        {
        public:
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
            Msg *msg_;
        };

        template <typename Msg>
        class BinaryMessage<Msg, AllocationPolicyAllocateOnHeap>
        {
        public:
            BinaryMessage() = default;
            [[nodiscard]] constexpr const Msg &msg() const noexcept { return *msg_.get(); }
            [[nodiscard]] constexpr Msg &msg() noexcept { return *msg_.get(); }
            [[nodiscard]] constexpr std::string_view data() const noexcept
            {
                std::string_view data{reinterpret_cast<char *>(msg_.get()), sizeof(Msg)};
                return data;
            }

        private:
            Msg *msg_{std::make_unique<Msg>()};
        };

        template <typename Msg>
        class BinaryMessageAcquireByReference
        {
        public:
            explicit constexpr BinaryMessageAcquireByReference(Msg &msg) noexcept
                : msg_{&msg} {}
            explicit constexpr BinaryMessageAcquireByReference(std::string_view data) noexcept
                : msg_{(reinterpret_cast<Msg *>(const_cast<char *>(data.data())))} {}

            [[nodiscard]] constexpr const Msg &msg() const noexcept { return *msg_; }
            [[nodiscard]] constexpr Msg &msg() noexcept { return *msg_; }
            [[nodiscard]] constexpr std::string_view data() const noexcept
            {
                std::string_view data{reinterpret_cast<char *>(msg_), sizeof(Msg)};
                return data;
            }

        private:
            Msg *msg_{};
        };

        template <typename Msg>
        class BinaryMessageAllocateOnHeap
        {
        public:
            BinaryMessageAllocateOnHeap() = default;
            [[nodiscard]] constexpr const Msg &msg() const noexcept { return *msg_.get(); }
            [[nodiscard]] constexpr Msg &msg() noexcept { return *msg_.get(); }
            [[nodiscard]] constexpr std::string_view data() const noexcept
            {
                std::string_view data{reinterpret_cast<char *>(msg_.get()), sizeof(Msg)};
                return data;
            }

        private:
            Msg *msg_{std::make_unique<Msg>()};
        };

        template <typename Msg, typename T>
        struct BaseSelector;

        template <typename Msg>
        struct BaseSelector<Msg, AllocationPolicyAcquireByReference>
        {
            using type = BinaryMessageAcquireByReference<Msg>;
        };

        template <typename Msg>
        struct BaseSelector<Msg, AllocationPolicyAllocateOnHeap>
        {
            using type = BinaryMessageAllocateOnHeap<Msg>;
        };

        // template <typename Msg, typename AllocationPolicy = AllocationPolicyAcquireByReference>
        // struct NewOrderSingle : public BaseSelector<Msg, AllocationPolicy>::type, NewOrderSingle
        // {
        //     NewOrderSingle(typename std::enable_if<std::is_same<AllocationPolicy, AllocationPolicyAllocateOnHeap>::value>::type * = nullptr) noexcept
        //         : BinaryMessage<Msg>{} {}
        //     explicit constexpr NewOrderSingle(Msg &msg,
        //                                       typename std::enable_if<std::is_same_v<AllocationPolicy, AllocationPolicyAcquireByReference>>::type * = nullptr) noexcept
        //         : BinaryMessage<Msg>{msg} {}
        //     explicit constexpr NewOrderSingle(std::string_view data,
        //                                       typename std::enable_if<std::is_same_v<AllocationPolicy, AllocationPolicyAcquireByReference>>::type * = nullptr) noexcept
        //         : BinaryMessage<Msg>{data} {}
        // };
    }

    template <typename Msg>
    class BinaryMessage
    {
    public:
        explicit constexpr BinaryMessage(Msg &msg) noexcept
            : msg_{msg} {}
        explicit constexpr BinaryMessage(std::string_view data) noexcept
            : msg_{*(reinterpret_cast<Msg *>(const_cast<char *>(data.data())))} {}

        [[nodiscard]] constexpr const Msg &msg() const noexcept { return msg_; }
        [[nodiscard]] constexpr Msg &msg() noexcept { return msg_; }
        [[nodiscard]] constexpr std::string_view data() const noexcept
        {
            std::string_view data{reinterpret_cast<char *>(&msg_), sizeof(Msg)};
            return data;
        }

    private:
        Msg &msg_{};
    };

    template <typename Msg>
    struct NewOrderSingle : public BinaryMessage<Msg>, framework::message::NewOrderSingle
    {
        explicit constexpr NewOrderSingle(Msg &msg) noexcept
            : BinaryMessage<Msg>{msg} {}
        explicit constexpr NewOrderSingle(std::string_view data) noexcept
            : BinaryMessage<Msg>{data} {}
    };

    template <typename Msg>
    struct CancelReplaceRequest : public BinaryMessage<Msg>, max::framework::message::CancelReplaceRequest
    {
        explicit constexpr CancelReplaceRequest(Msg &msg) noexcept
            : BinaryMessage<Msg>{msg} {}
        explicit constexpr CancelReplaceRequest(std::string_view data) noexcept
            : BinaryMessage<Msg>{data} {}
    };

    template <typename Msg>
    struct CancelRequest : public BinaryMessage<Msg>, max::framework::message::CancelRequest
    {
        explicit constexpr CancelRequest(Msg &msg) noexcept
            : BinaryMessage<Msg>{msg} {}
        explicit constexpr CancelRequest(std::string_view data) noexcept
            : BinaryMessage<Msg>{data} {}
    };

    template <typename Msg>
    struct ExecutionReport : public BinaryMessage<Msg>, max::framework::message::ExecutionReport
    {
        explicit constexpr ExecutionReport(Msg &msg) noexcept
            : BinaryMessage<Msg>{msg} {}
        explicit constexpr ExecutionReport(std::string_view data) noexcept
            : BinaryMessage<Msg>{data} {}
    };

    template <typename Msg>
    struct CancelReject : public BinaryMessage<Msg>, max::framework::message::CancelReject
    {
        explicit constexpr CancelReject(Msg &msg) noexcept
            : BinaryMessage<Msg>{msg} {}
        explicit constexpr CancelReject(std::string_view data) noexcept
            : BinaryMessage<Msg>{data} {}
    };
}