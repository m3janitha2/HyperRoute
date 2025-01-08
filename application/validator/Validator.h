#pragma once

#include <framework/utility/RejectInfo.h>
#include <application/message/core/Messages.h>
#include <application/validator/StatelessCheck01.h>
#include <type_traits>
#include <cstdint>

namespace hyper
{
    using RejectInfo = framework::RejectInfo;

    /* Message interface required by the validator */
    template <typename Msg>
    concept ValidatorMsg = requires(Msg msg) {
        { msg.price() } -> std::convertible_to<std::uint64_t>;
        { msg.size() } -> std::convertible_to<std::uint64_t>;
    };

    /* Example of a validator */
    class Validator
    {
    public:
        template <ValidatorMsg Msg>
        RejectInfo validate(Msg &msg)
        {
            std::uint64_t limit{dummy_limit_};
            if (!check_01_.is_enabled()) [[unlikely]]
                return validate(check_01_, msg, msg.price(), msg.size(), limit);

            return RejectInfo {};
        }

    private:
        template <typename Check, ValidatorMsg Msg, typename... Args>
        RejectInfo validate(Check &check, Msg &, Args &&...args)
        {
            if constexpr (std::derived_from<Msg, trading::NewOrderSingle>)
                return check.on_new_order_single(std::forward<Args>(args)...);
            else if constexpr (std::derived_from<Msg, trading::CancelReplaceRequest>)
                return check.on_cancel_replace_request(std::forward<Args>(args)...);
            else if constexpr (std::derived_from<Msg, trading::CancelRequest>)
                return check.on_cancel_request(std::forward<Args>(args)...);
            else if constexpr (std::derived_from<Msg, trading::ExecutionReport>)
                return check.on_execution_report(std::forward<Args>(args)...);
            else if constexpr (std::derived_from<Msg, trading::CancelReject>)
                return check.on_cancel_reject(std::forward<Args>(args)...);
        };

        /* Checks */
        StatelessCheck01 check_01_{};
        /* Initialize limits and data sources here. */
        volatile std::uint64_t dummy_limit_{10000000};
    };
}