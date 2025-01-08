#pragma once

#include <framework/utility/Types.h>

namespace hyper::framework
{
    /* Unique ID generator */
    class UIDGenerator
    {
    public:
        UIDGenerator(const UIDGenerator &) = delete;
        UIDGenerator &operator=(const UIDGenerator &) = delete;

        static UIDGenerator &instance()
        {
            static UIDGenerator instance{};
            return instance;
        }

        [[nodiscard]] UID get_next_uid() const noexcept { return ++uid_; }

    private:
        UIDGenerator() = default;
        ~UIDGenerator() = default;

        inline static constinit UID uid_{0};
    };
}