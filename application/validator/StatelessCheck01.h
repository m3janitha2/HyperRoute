#pragma once

#include <framework/utility/RejectInfo.h>
#include <application/message/core/Messages.h>
#include <type_traits>
#include <cstdint>

namespace hyper
{
    using RejectInfo = framework::RejectInfo;

    /* This is a dummy check to demonstrate usage */
    class StatelessCheck01
    {
    public:
        RejectInfo on_new_order_single(std::uint64_t price, std::uint64_t size, std::uint64_t limit)
        {
            return validate(price, size, limit);
        }

        RejectInfo on_cancel_replace_request(std::uint64_t price, std::uint64_t size, std::uint64_t limit)
        {
            return validate(price, size, limit);
        }

        RejectInfo on_cancel_request(std::uint64_t price, std::uint64_t size, std::uint64_t limit)
        {
            return RejectInfo{};
        }

        RejectInfo on_execution_report(std::uint64_t price, std::uint64_t size, std::uint64_t limit)
        {
            return RejectInfo{};
        }

        RejectInfo on_cancel_reject(std::uint64_t price, std::uint64_t size, std::uint64_t limit)
        {
            return RejectInfo{};
        }

        [[nodiscard]] constexpr bool is_enabled() const noexcept { return enabled_; }

    private:
        RejectInfo validate(std::uint64_t price, std::uint64_t size, std::uint64_t limit)
        {
            if (price * size > limit) [[unlikely]]
                return RejectInfo{"Validation Failed Check 01",
                                  framework::InteranlRejectCode::Validation_Failed_Check_01};

            return RejectInfo{};
        }

        bool enabled_{true};
    };
}