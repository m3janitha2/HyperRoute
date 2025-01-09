#pragma once

#include <framework/utility/CrtpBase.h>
#include <framework/utility/RejectInfo.h>
#include <framework/enricher/SourceEnricher.h>
#include <framework/router/SourceRouter.h>
#include <framework/transport/Transport.h>
#include <framework/message/Message.h>
#include <framework/application_dependency/DestinationRouters.h>
#include <type_traits>
#include <concepts>
#include <string_view>

namespace hyper::framework
{
    /* Source-side application message session abstraction. */
    /* - Receive messages from the source protocol and forward them to the router */
    /* - Receive messages from the destination and forward them to the transport */
    template <typename SessionImpl>
    class SourceSession : public CrtpBase<SessionImpl>
    {
    public:
        explicit SourceSession(Transport &transport,
                               const DestinationRouterPtrVarient &destination_router,
                               const SourceRouter &source_router)
            : transport_{transport},
              destination_router_{const_cast<DestinationRouterPtrVarient&>(destination_router)},
              source_router_{const_cast<SourceRouter&>(source_router)} {}

        SourceSession(const SourceSession &) = delete;
        SourceSession &operator=(const SourceSession &) = delete;

        /* TransportEvents */
        void on_connect() noexcept;
        void on_disconnect() noexcept;

        template <typename Msg>
        void procoess_message_from_transport(Msg &msg) noexcept;
        template <typename Msg>
        RejectInfo procoess_message_to_transport(Msg &msg) noexcept;
        template <typename Msg>
        RejectInfo enrich_message_from_transport(Msg &msg) noexcept;
        template <typename Msg>
        RejectInfo enrich_message_to_transport(Msg &msg) noexcept;
        template <typename Msg>
        RejectInfo send_message_to_peer(Msg &msg) noexcept;
        template <typename Msg>
        RejectInfo send_message_to_transport(Msg &msg) noexcept;
        template <typename Msg>
        void on_message_from_transport(Msg &msg);
        template <typename Msg>
        RejectInfo on_message_from_peer(Msg &msg);
        template <typename Msg>
        void rejecet_message_from_transport(Msg &msg, RejectInfo &reject_info);
        template <typename Msg>
        void update_routing_info(Msg &msg) noexcept;

        [[nodiscard]] constexpr bool is_connected() const noexcept { return connected_; }

    private:
        DestinationRouterPtrVarient &destination_router_;
        SourceRouter &source_router_;
        SourceEnricher enricher_{};
        Transport &transport_;
        bool connected_{true};
    };

    template <typename SessionImpl>
    inline void SourceSession<SessionImpl>::on_connect() noexcept
    {
        this->impl().on_connect_impl();
    }

    template <typename SessionImpl>
    inline void SourceSession<SessionImpl>::on_disconnect() noexcept
    {
        this->impl().on_disconnect_impl();
    }

    template <typename SessionImpl>
    template <typename Msg>
    inline void SourceSession<SessionImpl>::procoess_message_from_transport(Msg &msg) noexcept
    {
        if (auto reject_info = enrich_message_from_transport(msg);
            reject_info != true) [[unlikely]]
            rejecet_message_from_transport(msg, reject_info);

        if (auto reject_info = send_message_to_peer(msg);
            reject_info != true) [[unlikely]]
            rejecet_message_from_transport(msg, reject_info);

        /* Message sent out from the destination session */

        update_routing_info(msg);

        // Persist the recevied message for session recovery.
        // Publish the recevied message to other channels.
    }

    template <typename SessionImpl>
    template <typename Msg>
    inline RejectInfo SourceSession<SessionImpl>::procoess_message_to_transport(Msg &msg) noexcept
    {
        if (auto reject_info = enrich_message_to_transport(msg);
            reject_info != true) [[unlikely]]
            return reject_info;

        if (auto reject_info = send_message_to_transport(msg);
            reject_info != true) [[unlikely]]
            return reject_info;

        // Persist the sent message for session recovery.
        // Publish the sent message to other channels.

        log_message_info(msg);

        return RejectInfo{};
    }

    template <typename SessionImpl>
    template <typename Msg>
    inline RejectInfo SourceSession<SessionImpl>::enrich_message_from_transport(Msg &msg) noexcept
    {
        return enricher_.enrich_message_from_transport(msg);
    }

    template <typename SessionImpl>
    template <typename Msg>
    inline RejectInfo SourceSession<SessionImpl>::enrich_message_to_transport(Msg &msg) noexcept
    {
        return enricher_.enrich_message_to_transport(msg);
    }

    template <typename SessionImpl>
    template <typename Msg>
    inline RejectInfo SourceSession<SessionImpl>::send_message_to_peer(Msg &msg) noexcept
    {
        return std::visit([&msg]<typename Router>(Router &&router)
                          { return std::forward<Router>(router)->on_message_from_source(msg); },
                          destination_router_);
    }

    template <typename SessionImpl>
    template <typename Msg>
    inline RejectInfo SourceSession<SessionImpl>::send_message_to_transport(Msg &msg) noexcept
    {
        msg.update_out_timestamp();
        return transport_.send_data(msg.data());
    }

    template <typename SessionImpl>
    template <typename Msg>
    inline void SourceSession<SessionImpl>::on_message_from_transport(Msg &msg)
    {
        this->impl().on_message_from_transport_impl(msg);
    }

    template <typename SessionImpl>
    template <typename Msg>
    inline RejectInfo SourceSession<SessionImpl>::on_message_from_peer(Msg &msg)
    {
        return this->impl().on_message_from_peer_impl(msg);
    }

    template <typename SessionImpl>
    template <typename Msg>
    inline void SourceSession<SessionImpl>::rejecet_message_from_transport(Msg &msg, RejectInfo &reject_info)
    {
        this->impl().rejecet_message_from_transport_impl(msg, reject_info);
    }

    template <typename SessionImpl>
    template <typename Msg>
    inline void SourceSession<SessionImpl>::update_routing_info(Msg &msg) noexcept
    {
        if constexpr (std::derived_from<Msg, framework::message::FirstEvent>)
        {
            SourceSessionPtrVarient source_session_varient{&(this->impl())};
            source_router_.update_routing_info(msg.uid(), source_session_varient);
        }

        return this->impl().update_routing_info_impl(msg);
    }
}