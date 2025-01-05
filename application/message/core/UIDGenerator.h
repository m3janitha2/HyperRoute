#pragma once

#include <application/message/core/Types.h>

namespace max::message
{
    class UIDGenerator
    {
    public:
        UIDGenerator(const UIDGenerator &) = delete;

        static UIDGenerator &instance()
        {
            static UIDGenerator instance{};
            return instance;
        }

        [[nodiscard]] UID get_next_uid() noexcept { return ++uid_; }

    private:
        UIDGenerator() = default;
        inline static constinit UID uid_{0};
    };
}