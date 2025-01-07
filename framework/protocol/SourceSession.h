#pragma once

#include <framework/utility/CrtpBase.h>
#include <framework/protocol/RejectInfo.h>
#include <framework/enricher/SourceEnricher.h>
#include <framework/router/SourceRouter.h>
#include <framework/transport/Transport.h>
#include <framework/message/Message.h>
#include <framework/router/DestinationRouters.h>
#include <type_traits>
#include <concepts>
#include <string_view>

namespace hyper::framework
{
    template <typename Impl>
    concept TransportEvents = requires(Impl impl) {
        impl.on_connect_impl();
        impl.on_disconnect_impl();
    };

    template <typename SessionImpl>
    class SourceSession : public CrtpBase<SessionImpl>
    {
    public:
        explicit SourceSession(DestinationRouterPtrVarient &destination_router,
                               SourceRouter &source_router,
                               Transport &transport)
            : destination_router_{destination_router},
              source_router_{source_router},
              transport_{transport}
        {
            static_assert(TransportEvents<SessionImpl>);
        }

        SourceSession(const SourceSession &) = delete;
        SourceSession &operator=(const SourceSession &) = delete;

        /* TransportEvents */
        void on_connect() { this->impl().on_connect_impl(); }
        void on_disconnect() { this->impl().on_disconnect_impl(); }

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
        void on_message_from_transport(Msg &msg) { this->impl().on_message_from_transport_impl(msg); }
        template <typename Msg>
        RejectInfo on_message_from_peer(Msg &msg) { return this->impl().on_message_from_peer_impl(msg); }
        template <typename Msg>
        void rejecet_message_from_transport(Msg &msg, RejectInfo &reject_info) { this->impl().rejecet_message_from_transport_impl(msg, reject_info); }
        template <typename Msg>
        void update_routing_info(Msg &msg) noexcept;

    private:
        DestinationRouterPtrVarient &destination_router_;
        SourceRouter &source_router_;
        SourceEnricher enricher_{};
        Transport &transport_;
    };

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

        // Persist SourceSession message
        // Publish SourceSession message downstream
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
        std::cout << "send_message_to_transport:" << msg << std::endl;
        msg.update_out_timestamp();
        return transport_.send_data(msg.data());
    }

    template <typename SessionImpl>
    template <typename Msg>
    inline void SourceSession<SessionImpl>::update_routing_info(Msg &msg) noexcept
    {
        if constexpr (std::derived_from<Msg, framework::message::FirstEvent>)
        {
            std::cout << "update_routing_info: NewOrderSingle. uid:" << msg.uid() << std::endl;
            SourceSessionPtrVarient source_session_varient{&(this->impl())};
            source_router_.update_reverse_routing(msg.uid(), source_session_varient);
        }

        return this->impl().update_routing_info_impl(msg);
    }
}