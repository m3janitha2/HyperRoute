#pragma once

#include <application/message/core/Types.h>

namespace max::message
{
    class UIDGenerator
    {
    public:
        UIDGenerator(const UIDGenerator &) = delete;

        UIDGenerator &instance()
        {
            static UIDGenerator instance{};
            return instance;
        }

        [[nodiscard]] constexpr UID get_next_uid() noexcept { return ++uid_; }

    private:
        UIDGenerator() = default;
        static constinit UID uid_{0};
    };
}