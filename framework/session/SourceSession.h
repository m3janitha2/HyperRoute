#pragma once

#include <framework/session/Session.h>
#include <framework/message/Message.h>
#include <framework/utility/RejectInfo.h>
#include <framework/enricher/SourceEnricher.h>
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

    template <typename SessionImpl>
    class SourceSession : public Session<SessionImpl>
    {
    public:
        explicit SourceSession(Transport &transport,
                               const DestinationRouterPtrVarient &destination_router,
                               const SourceRouter &source_router)
            : Session<SessionImpl>{transport},
              destination_router_{const_cast<DestinationRouterPtrVarient &>(destination_router)},
              source_router_{const_cast<SourceRouter &>(source_router)} {}

        template <MessageInf Msg>
        inline void on_message_from_transport_impl(Msg &msg) noexcept;
        template <MessageInf Msg>
        RejectInfo on_message_from_peer(Msg &msg) noexcept;
        template <MessageInf Msg>
        RejectInfo procoess_message_to_transport(Msg &msg) noexcept;
        template <MessageInf Msg>
        void procoess_message_from_transport(Msg &msg) noexcept;
        template <MessageInf Msg>
        RejectInfo enrich_message_to_transport(Msg &msg) noexcept;
        template <MessageInf Msg>
        RejectInfo enrich_message_from_transport(Msg &msg) noexcept;
        template <MessageInf Msg>
        RejectInfo handle_message_from_transport(Msg &msg) noexcept;
        template <MessageInf Msg>
        RejectInfo send_message_to_peer(Msg &msg) noexcept;
        template <MessageInf Msg>
        void reject_message_from_transport(Msg &msg, RejectInfo &reject_info) noexcept;
        template <MessageInf Msg>
        void update_source_routing_info(Msg &msg) noexcept;

    private:
        DestinationRouterPtrVarient &destination_router_;
        SourceRouter &source_router_;
        SourceEnricher enricher_{};
    };

    template <typename SessionImpl>
    template <MessageInf Msg>
    inline void SourceSession<SessionImpl>::on_message_from_transport_impl(Msg &msg) noexcept
    {
        procoess_message_from_transport(msg);
    }

    template <typename SessionImpl>
    template <MessageInf Msg>
    inline RejectInfo SourceSession<SessionImpl>::on_message_from_peer(Msg &msg) noexcept
    {
        return this->impl().on_message_from_peer_impl(msg);
    }

    template <typename SessionImpl>
    template <MessageInf Msg>
    inline RejectInfo SourceSession<SessionImpl>::procoess_message_to_transport(Msg &msg) noexcept
    {
        if (auto reject_info = enrich_message_to_transport(msg);
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

    template <typename SessionImpl>
    template <MessageInf Msg>
    inline void SourceSession<SessionImpl>::procoess_message_from_transport(Msg &msg) noexcept
    {
        if (auto reject_info = enrich_message_from_transport(msg);
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
        // Publish the recevied message to other channels.
    }

    template <typename SessionImpl>
    template <MessageInf Msg>
    inline RejectInfo SourceSession<SessionImpl>::enrich_message_to_transport(Msg &msg) noexcept
    {
        return enricher_.enrich_message_to_transport(msg);
    }

    template <typename SessionImpl>
    template <MessageInf Msg>
    inline RejectInfo SourceSession<SessionImpl>::enrich_message_from_transport(Msg &msg) noexcept
    {
        return enricher_.enrich_message_from_transport(msg);
    }

    template <typename SessionImpl>
    template <MessageInf Msg>
    inline RejectInfo SourceSession<SessionImpl>::handle_message_from_transport(Msg &msg) noexcept
    {
        return this->impl().handle_message_from_transport_impl(msg);
    }

    template <typename SessionImpl>
    template <MessageInf Msg>
    inline RejectInfo SourceSession<SessionImpl>::send_message_to_peer(Msg &msg) noexcept
    {
        return std::visit([&msg]<typename Router>(Router &&router)
                          { return std::forward<Router>(router)->on_message_from_source(msg); },
                          destination_router_);
    }

    template <typename SessionImpl>
    template <MessageInf Msg>
    inline void SourceSession<SessionImpl>::reject_message_from_transport(Msg &msg, RejectInfo &reject_info) noexcept
    {
        this->impl().reject_message_from_transport_impl(msg, reject_info);
    }

    template <typename SessionImpl>
    template <MessageInf Msg>
    inline void SourceSession<SessionImpl>::update_source_routing_info(Msg &msg) noexcept
    {
        if constexpr (std::derived_from<Msg, FirstEvent>)
        {
            SourceSessionPtrVarient source_session_varient{&(this->impl())};
            source_router_.update_source_routing_info(msg.uid(), source_session_varient);
        }

        return this->impl().update_routing_info_impl(msg);
    }

}
