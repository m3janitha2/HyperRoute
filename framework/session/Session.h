#pragma once

#include <framework/utility/CrtpBase.h>
#include <framework/utility/RejectInfo.h>
#include <framework/message/Message.h>
#include <framework/transport/Transport.h>
#include <framework/config/Configuration.h>
#include <type_traits>
#include <concepts>

namespace hyper::framework
{
    /* application message session abstraction. */
    template <typename SessionImpl>
    concept SessionInf = requires(SessionImpl ds) {
        { ds.impl().on_connect_impl() } -> std::same_as<void>;
        { ds.impl().on_disconnect_impl() } -> std::same_as<void>;
    };

    template <typename SessionImpl>
    class Session : public CrtpBase<SessionImpl>
    {
    public:
        explicit Session(Transport &transport)
            : transport_(transport)
        {
            static_assert(SessionInf<Session<SessionImpl>>,
                          "The Session implementation does not satisfy SessionInf");
        }

        Session(const Session &) = delete;
        Session &operator=(const Session &) = delete;

        /* SessionInf */
        void on_connect() noexcept;
        void on_disconnect() noexcept;
        [[nodiscard]] constexpr bool is_connected() const noexcept { return connected_; }

        template <MessageInf Msg>
        RejectInfo send_message_to_transport(Msg &msg) const noexcept;
        template <MessageInf Msg>
        void on_message_from_transport(Msg &msg) noexcept;

    private:
        bool connected_{true};
        Transport &transport_;
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

    template <typename SessionImpl>
    template <MessageInf Msg>
    inline RejectInfo Session<SessionImpl>::send_message_to_transport(Msg &msg) const noexcept
    {
        msg.update_out_timestamp();
        return transport_.send_data(msg.data());
    }

    template <typename SessionImpl>
    template <MessageInf Msg>
    inline void Session<SessionImpl>::on_message_from_transport(Msg &msg) noexcept
    {
        this->impl().on_message_from_transport_impl(msg);
    }
}
