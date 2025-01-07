#pragma once

namespace hyper::framework
{
    template <typename T>
    struct CrtpBase
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