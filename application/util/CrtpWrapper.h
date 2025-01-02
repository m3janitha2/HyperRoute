#pragma once

namespace max::core
{
    template <typename T>
    struct CrtpWrapper
    {
        T &impl() noexcept
        {
            return static_cast<T &>(*this);
        }

        const T &impl() const noexcept
        {
            return static_cast<const T &>(*this);
        }
    };
}