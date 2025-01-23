#pragma once

#include <framework/message/Message.h>
#include <ostream>
#include <string_view>
#include <chrono>
#include <memory>

namespace hyper::framework
{
    /* Binary message abstraction. */
    template <typename Msg>
    class BinaryMessage : public Message
    {
    public:
        explicit constexpr BinaryMessage(Msg &msg,
                                         Timestamp in_timestamp = TimestampClock::now()) noexcept
            : Message{in_timestamp},
              msg_{msg} {}
        explicit constexpr BinaryMessage(std::string_view data,
                                         Timestamp in_timestamp = TimestampClock::now()) noexcept
            : Message{in_timestamp},
              msg_{*(reinterpret_cast<Msg *>(const_cast<char *>(data.data())))} {}

        BinaryMessage(const BinaryMessage &) = delete;
        BinaryMessage &operator=(const BinaryMessage &) = delete;

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

    /* todox: allocation policy */
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
        // struct NewOrderSingle : public BaseSelector<Msg, AllocationPolicy>::type, framework::message::NewOrderSingle
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
}