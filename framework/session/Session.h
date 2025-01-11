#pragma once

#include <framework/utility/CrtpBase.h>
#include <framework/utility/RejectInfo.h>
#include <type_traits>
#include <concepts>

namespace hyper::framework
{
    /* application message session abstraction. */
    template <typename SessionImpl>
    concept SessionInf = requires(SessionImpl ds) {
        { ds.on_connect_impl() } -> std::same_as<void>;
        { ds.on_disconnect_impl() } -> std::same_as<void>;
    };

    template <typename SessionImpl>
    class Session : public CrtpBase<SessionImpl>
    {
    public:
        explicit Session() = default;

        Session(const Session &) = delete;
        Session &operator=(const Session &) = delete;

        /* SessionInf */
        void on_connect() noexcept;
        void on_disconnect() noexcept;
        [[nodiscard]] constexpr bool is_connected() const noexcept { return connected_; }

    private:
        bool connected_{true};
    };

    template <typename SessionImpl>
    inline void Session<SessionImpl>::on_connect() noexcept
    {
        this->impl().on_connect_impl();
    }

    template <typename SessionImpl>
    inline void Session<SessionImpl>::on_disconnect() noexcept
    {
        this->impl().on_disconnect_impl();
    }
}
