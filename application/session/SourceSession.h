#pragma once

#include <application/util/CrtpWrapper.h>
#include <application/session/RejectInfo.h>
#include <application/enricher/ClientEnricher.h>
#include <application/router/DestinatinRouters.h>
#include <application/router/ReverseRouter.h>
#include <application/transport/Transport.h>
#include <application/message/core/Messages.h>

#include <type_traits>
#include <concepts>
#include <string_view>

namespace max
{
    template <typename Impl>
    concept TransportEvents = requires(Impl impl) {
        impl.on_connect_impl();
        impl.on_disconnect_impl();
    };

    template <typename SessionImpl>
    class SourceSessionBase : public core::CrtpWrapper<SessionImpl>
    {
    public:
        explicit SourceSessionBase(DestinatinRouterVarient &forward_router,
                                   ReverseRouter &reverse_router,
                                   Transport &transport)
            : forward_router_{forward_router},
              reverse_router_{reverse_router},
              transport_{transport}
        {
            static_assert(TransportEvents<SessionImpl>);
        }

        SourceSessionBase(const SourceSessionBase &) = delete;

        /* TransportEvents */
        void on_connect() { this->impl().on_connect_impl(); }
        void on_disconnect() { this->impl().on_disconnect_impl(); }

        template <typename Msg>
        void procoess_message_from_transport(Msg &msg);
        template <typename Msg>
        RejectInfo procoess_message_to_transport(Msg &msg);
        template <typename Msg>
        RejectInfo enrich_message_from_transport(Msg &msg);
        template <typename Msg>
        RejectInfo enrich_message_to_transport(Msg &msg);
        template <typename Msg>
        RejectInfo send_message_to_peer(Msg &msg);
        template <typename Msg>
        RejectInfo send_message_to_transport(Msg &msg);
        template <typename Msg>
        void on_message_from_transport(Msg &msg) { this->impl().on_message_from_transport_impl(msg); }
        template <typename Msg>
        RejectInfo on_message_from_peer(Msg &msg) { return this->impl().on_message_from_peer_impl(msg); }
        template <typename Msg>
        void rejecet_message_from_transport(Msg &msg, RejectInfo &reject_info) { this->impl().rejecet_message_from_transport_impl(msg, reject_info); }
        template <typename Msg>
        void update_routing_info(Msg &msg);

    private:
        DestinatinRouterVarient &forward_router_;
        ReverseRouter &reverse_router_;
        ClientEnricher enricher_{};
        Transport &transport_;
    };

    template <typename SessionImpl>
    template <typename Msg>
    inline void SourceSessionBase<SessionImpl>::procoess_message_from_transport(Msg &msg)
    {
        if (auto reject_info = enrich_message_from_transport(msg); reject_info != true)
        {
            rejecet_message_from_transport(msg, reject_info);
        }

        if (auto reject_info = send_message_to_peer(msg); reject_info != true)
        {
            rejecet_message_from_transport(msg, reject_info);
        }

        /* Message sent out from the destination session */

        update_routing_info(msg);

        // Persist SourceSession message
        // Publish SourceSession message downstream
    }

    template <typename SessionImpl>
    template <typename Msg>
    inline RejectInfo SourceSessionBase<SessionImpl>::procoess_message_to_transport(Msg &msg)
    {
        if (auto reject_info = enrich_message_to_transport(msg); reject_info != true)
            return reject_info;

        if (auto reject_info = send_message_to_transport(msg); reject_info != true)
            return reject_info;

        return RejectInfo{};
    }

    template <typename SessionImpl>
    template <typename Msg>
    inline RejectInfo SourceSessionBase<SessionImpl>::enrich_message_from_transport(Msg &msg)
    {
        return enricher_.enrich_message_from_transport(msg);
    }

    template <typename SessionImpl>
    template <typename Msg>
    inline RejectInfo SourceSessionBase<SessionImpl>::enrich_message_to_transport(Msg &msg)
    {
        return enricher_.enrich_message_to_transport(msg);
    }

    template <typename SessionImpl>
    template <typename Msg>
    inline RejectInfo SourceSessionBase<SessionImpl>::send_message_to_peer(Msg &msg)
    {
        return std::visit([&msg]<typename Router>(Router &&router)
                          { return std::forward<decltype(router)>(router).on_message_from_source(msg); },
                          forward_router_);
    }

    template <typename SessionImpl>
    template <typename Msg>
    inline RejectInfo SourceSessionBase<SessionImpl>::send_message_to_transport(Msg &msg)
    {
        std::cout << "send_message_to_transport:" << msg << std::endl;
        return transport_.send_data(msg.data());
    }

    template <typename SessionImpl>
    template <typename Msg>
    inline void SourceSessionBase<SessionImpl>::update_routing_info(Msg &msg)
    {
        if constexpr (std::derived_from<Msg, message::core::NewOrderSingle>)
        {
            std::cout << "update_routing_info: NewOrderSingle. uid:" << msg.uid() << std::endl;
            SourceSessionPtrVarient client_session_varient{&(this->impl())};
            reverse_router_.update_reverse_routing(msg.uid(), client_session_varient);
        }

        return this->impl().update_routing_info_impl(msg);
    }
}