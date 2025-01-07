#pragma once

#include <framework/protocol/RejectInfo.h>
#include <application/message/core/Messages.h>
#include <type_traits>
#include <cstdint>

namespace hyper
{
    template <typename Msg>
    concept ValidatorMsg = requires(Msg msg) {
        { msg.price() } -> std::convertible_to<std::uint64_t>;
        { msg.size() } -> std::convertible_to<std::uint64_t>;
    };

    using RejectInfo = framework::RejectInfo;

    /* Dummy check */
    struct Check_01
    {
        RejectInfo validate(std::uint64_t price, std::uint64_t size, std::uint64_t limit)
        {
            if (price * size > limit) [[unlikely]]
                return RejectInfo{"Validation Failed Check 01",
                                  framework::InteranlRejectCode::Validation_Failed_Check_01};

            return RejectInfo{};
        }

        RejectInfo ignore(std::uint64_t price, std::uint64_t size, std::uint64_t limit)
        {
            return RejectInfo{};
        }

        RejectInfo on_new_order_single(std::uint64_t price, std::uint64_t size, std::uint64_t limit)
        {
            return validate(price, size, limit);
        }

        RejectInfo on_new_cancel_replace(std::uint64_t price, std::uint64_t size, std::uint64_t limit)
        {
            return validate(price, size, limit);
        }
    };

    class Validator
    {
    public:
        template <ValidatorMsg Msg>
        RejectInfo validate(Msg &msg)
        {
            if constexpr (std::derived_from<Msg, trading::NewOrderSingle>)
                return check_01_.on_new_order_single(msg.price(), msg.size(), 1000000000);
            else if constexpr (std::derived_from<Msg, trading::CancelReplaceRequest>)
                return check_01_.on_new_cancel_replace(msg.price(), msg.size(), 1000000000);
            else
                return check_01_.ignore(msg.price(), msg.size(), 1000000000);
        }

    private:
        Check_01 check_01_{};
    };
}