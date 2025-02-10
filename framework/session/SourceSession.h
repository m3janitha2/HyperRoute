#pragma once

#include <framework/session/Session.h>
#include <framework/message/Message.h>
#include <framework/utility/RejectInfo.h>
#include <framework/transform/SourceTransform.h>
#include <framework/router/SourceRouter.h>
#include <framework/application_dependency/DestinationRouters.h>
#include <string_view>
#include <type_traits>

namespace hyper::framework
{
    /* Source-side application message session abstraction. */
    /* - Receive messages from the source protocol and forward them to the router */
    /* - Receive messages from the destination and forward them to the transport */

    template <typename SessionImpl, typename SourceMsg>
    concept SourceSessionInf = requires(SessionImpl ds,
                                        SourceMsg src_msg,
                                        RejectInfo reject_info) {
        { ds.impl().on_message_from_transport_impl(src_msg) } -> std::same_as<void>;
        { ds.impl().on_message_from_peer_impl(src_msg) } -> std::same_as<RejectInfo>;
        { ds.impl().reject_message_from_transport_impl(src_msg, reject_info) } -> std::same_as<void>;
        { ds.impl().update_destination_routing_info_impl(src_msg) } -> std::same_as<void>;
    };

    template <typename SessionImpl, typename Protocol>
    class SourceSession : public Session<SessionImpl, Protocol>
    {
    public:
        explicit SourceSession(Protocol &protocol,
                               const DestinationRouterPtrVariant &destination_router,
                               SourceRouter &source_router)
            : Session<SessionImpl, Protocol>{protocol},
              destination_router_{destination_router},
              source_router_{source_router} {}

        template <MessageInf Msg>
        void on_message_from_transport_impl(Msg &msg) const noexcept;
        template <MessageInf Msg>
        RejectInfo on_message_from_peer(Msg &msg) const noexcept;
        template <MessageInf Msg>
        RejectInfo procoess_message_to_transport(Msg &msg) const noexcept;
        template <MessageInf Msg>
        void procoess_message_from_transport(Msg &msg) const noexcept;
        template <MessageInf Msg>
        RejectInfo transform_message_to_transport(Msg &msg) const noexcept;
        template <MessageInf Msg>
        RejectInfo transform_message_from_transport(Msg &msg) const noexcept;
        template <MessageInf Msg>
        RejectInfo handle_message_from_transport(Msg &msg) const noexcept;
        template <MessageInf Msg>
        RejectInfo send_message_to_peer(Msg &msg) const noexcept;
        template <MessageInf Msg>
        void reject_message_from_transport(Msg &msg, RejectInfo &reject_info) const noexcept;
        template <MessageInf Msg>
        void update_source_routing_info(Msg &msg) const noexcept;

    private:
        DestinationRouterPtrVariant destination_router_;
        SourceRouter &source_router_;
        SourceTransform transform_{};
    };

    template <typename SessionImpl, typename Protocol>
    template <MessageInf Msg>
    inline void SourceSession<SessionImpl, Protocol>::on_message_from_transport_impl(Msg &msg) const noexcept
    {
        procoess_message_from_transport(msg);
    }

    template <typename SessionImpl, typename Protocol>
    template <MessageInf Msg>
    inline RejectInfo SourceSession<SessionImpl, Protocol>::on_message_from_peer(Msg &msg) const noexcept
    {
        return this->impl().on_message_from_peer_impl(msg);
    }

    template <typename SessionImpl, typename Protocol>
    template <MessageInf Msg>
    inline RejectInfo SourceSession<SessionImpl, Protocol>::procoess_message_to_transport(Msg &msg) const noexcept
    {
        if (auto reject_info = transform_message_to_transport(msg);
            reject_info != true) [[unlikely]]
            return reject_info;

        if (auto reject_info = this->impl().send_message_to_transport(msg);
            reject_info != true) [[unlikely]]
            return reject_info;

        // Persist the sent message for session recovery.
        // Publish the sent message to other channels.

        log_message_info(msg);

        return RejectInfo{};
    }

    template <typename SessionImpl, typename Protocol>
    template <MessageInf Msg>
    inline void SourceSession<SessionImpl, Protocol>::procoess_message_from_transport(Msg &msg) const noexcept
    {
        if (auto reject_info = transform_message_from_transport(msg);
            reject_info != true) [[unlikely]]
            reject_message_from_transport(msg, reject_info);

        if (auto reject_info = handle_message_from_transport(msg);
            reject_info != true) [[unlikely]]
            reject_message_from_transport(msg, reject_info);

        if (auto reject_info = send_message_to_peer(msg);
            reject_info != true) [[unlikely]]
            reject_message_from_transport(msg, reject_info);

        /* Message sent out from the destination session */

        update_source_routing_info(msg);

        // Persist the recevied message for session recovery.
    }

    template <typename SessionImpl, typename Protocol>
    template <MessageInf Msg>
    inline RejectInfo SourceSession<SessionImpl, Protocol>::transform_message_to_transport(Msg &msg) const noexcept
    {
        return transform_.transform_message_to_transport(msg);
    }

    template <typename SessionImpl, typename Protocol>
    template <MessageInf Msg>
    inline RejectInfo SourceSession<SessionImpl, Protocol>::transform_message_from_transport(Msg &msg) const noexcept
    {
        return transform_.transform_message_from_transport(msg);
    }

    template <typename SessionImpl, typename Protocol>
    template <MessageInf Msg>
    inline RejectInfo SourceSession<SessionImpl, Protocol>::handle_message_from_transport(Msg &msg) const noexcept
    {
        return this->impl().handle_message_from_transport_impl(msg);
    }

    template <typename SessionImpl, typename Protocol>
    template <MessageInf Msg>
    inline RejectInfo SourceSession<SessionImpl, Protocol>::send_message_to_peer(Msg &msg) const noexcept
    {
        return std::visit([&msg]<typename Router>(Router &&router)
                          { return std::forward<Router>(router)->on_message_from_source(msg); },
                          destination_router_);
    }

    template <typename SessionImpl, typename Protocol>
    template <MessageInf Msg>
    inline void SourceSession<SessionImpl, Protocol>::reject_message_from_transport(Msg &msg, RejectInfo &reject_info) const noexcept
    {
        this->impl().reject_message_from_transport_impl(msg, reject_info);
    }

    template <typename SessionImpl, typename Protocol>
    template <MessageInf Msg>
    inline void SourceSession<SessionImpl, Protocol>::update_source_routing_info(Msg &msg) const noexcept
    {
        if constexpr (std::derived_from<Msg, FirstEvent>)
        {
            SourceSessionPtrVariant source_session_Variant{&(this->impl())};
            source_router_.update_source_routing_info(msg.uid(), source_session_Variant);
        }

        return this->impl().update_routing_info_impl(msg);
    }

}
