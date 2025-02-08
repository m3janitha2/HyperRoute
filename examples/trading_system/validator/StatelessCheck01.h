#pragma once

#include <framework/utility/RejectInfo.h>
#include <examples/trading_system/message/core/Messages.h>
#include <type_traits>
#include <cstdint>

namespace hyper
{
    using RejectInfo = framework::RejectInfo;

    /* This is a dummy check to demonstrate usage */
    class StatelessCheck01
    {
    public:
        RejectInfo on_new_order_single(std::uint64_t price,
                                       std::uint64_t size,
                                       std::uint64_t limit) const noexcept
        {
            return validate(price, size, limit);
        }

        RejectInfo on_cancel_replace_request(std::uint64_t price,
                                             std::uint64_t size,
                                             std::uint64_t limit) const noexcept
        {
            return validate(price, size, limit);
        }

        RejectInfo on_cancel_request([[maybe_unused]] std::uint64_t price,
                                     [[maybe_unused]] std::uint64_t size,
                                     [[maybe_unused]] std::uint64_t limit) const noexcept
        {
            return RejectInfo{};
        }

        RejectInfo on_execution_report([[maybe_unused]] std::uint64_t price,
                                       [[maybe_unused]] std::uint64_t size,
                                       [[maybe_unused]] std::uint64_t limit) const noexcept
        {
            return RejectInfo{};
        }

        RejectInfo on_cancel_reject([[maybe_unused]] std::uint64_t price,
                                    [[maybe_unused]] std::uint64_t size,
                                    [[maybe_unused]] std::uint64_t limit) const noexcept
        {
            return RejectInfo{};
        }

        [[nodiscard]] constexpr bool is_enabled() const noexcept { return enabled_; }

    private:
        RejectInfo validate(std::uint64_t price, std::uint64_t size, std::uint64_t limit) const noexcept
        {
            if (price * size > limit) [[unlikely]]
                return RejectInfo{"Validation Failed Check 01",
                                  framework::InteranlRejectCode::Validatator_Validation_Failed};

            return RejectInfo{};
        }

        bool enabled_{true};
    };
}