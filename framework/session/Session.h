#pragma once

#include <framework/utility/CrtpBase.h>
#include <framework/utility/RejectInfo.h>
#include <framework/message/Message.h>
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

    template <typename SessionImpl, typename Protocol>
    class Session : public CrtpBase<SessionImpl>
    {
    public:
        explicit Session(Protocol &protocol)
            : protocol_(protocol)
        {
            static_assert(SessionInf<Session<SessionImpl, Protocol>>,
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
        void send_message_to_transport_ignoring_errors(Msg &msg) const noexcept;
        template <MessageInf Msg>
        void on_message_from_transport(Msg &msg) const noexcept;

    private:
        bool connected_{true};
        Protocol &protocol_;
    };

    template <typename SessionImpl, typename Protocol>
    inline void Session<SessionImpl, Protocol>::on_connect() noexcept
    {
        this->impl().on_connect_impl();
    }

    template <typename SessionImpl, typename Protocol>
    inline void Session<SessionImpl, Protocol>::on_disconnect() noexcept
    {
        this->impl().on_disconnect_impl();
    }

    template <typename SessionImpl, typename Protocol>
    template <MessageInf Msg>
    inline RejectInfo Session<SessionImpl, Protocol>::send_message_to_transport(Msg &msg) const noexcept
    {
        return protocol_.send_to_transport(msg);
    }

    template <typename SessionImpl, typename Protocol>
    template <MessageInf Msg>
    inline void Session<SessionImpl, Protocol>::send_message_to_transport_ignoring_errors(Msg &msg) const noexcept
    {
        if(auto reject_info = protocol_.send_to_transport(msg);
            reject_info != true) [[unlikely]]
            {
                std::cerr << "Failed to send message to transport" << std::endl;
            }
    }

    template <typename SessionImpl, typename Protocol>
    template <MessageInf Msg>
    inline void Session<SessionImpl, Protocol>::on_message_from_transport(Msg &msg) const noexcept
    {
        this->impl().on_message_from_transport_impl(msg);
    }
}
