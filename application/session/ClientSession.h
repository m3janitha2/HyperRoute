#pragma once

#include <application/util/CrtpWrapper.h>
#include <application/session/RejectInfo.h>
#include <application/enricher/ClientEnricher.h>
#include <application/router/ForwardRouters.h>
#include <application/router/ReverseRouter.h>
#include <application/transport/Transport.h>

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
    class ClientSessionBase : public core::CrtpWrapper<SessionImpl>
    {
    public:
        explicit ClientSessionBase(ForwardRouterVarient &forward_router,
                                   ReverseRouter &reverse_router,
                                   Transport &transport)
            : forward_router_{forward_router},
              reverse_router_{reverse_router},
              transport_{transport}
        {
            static_assert(TransportEvents<SessionImpl>);
        }

        /* TransportEvents */
        void on_connect() { this->impl().on_connect_impl(); }
        void on_disconnect() { this->impl().on_disconnect_impl(); }

        /* client session internal */
        template <typename Msg>
        void procoess_message_from_client(Msg &msg);
        template <typename Msg>
        RejectInfo procoess_message_to_client(Msg &msg);
        template <typename Msg>
        RejectInfo enrich_message_from_client(Msg &msg);
        template <typename Msg>
        RejectInfo enrich_message_to_client(Msg &msg);
        template <typename Msg>
        RejectInfo send_message_from_client(Msg &msg);
        template <typename Msg>
        RejectInfo send_message_to_client(Msg &msg);
        template <typename Msg>
        void on_message(Msg &msg) { this->impl().on_message_impl(msg); }
        template <typename Msg>
        RejectInfo on_venue_message(Msg &msg) { return this->impl().on_venue_message_impl(msg); }
        template <typename Msg>
        void reject_client_message(Msg &msg, RejectInfo &reject_info) { this->impl().reject_client_message_impl(msg, reject_info); }
        template <typename Msg>
        void update_routing_info(Msg &msg);

    private:
        ForwardRouterVarient &forward_router_;
        ReverseRouter &reverse_router_;
        ClientEnricher enricher_{};
        Transport &transport_;
    };

    template <typename SessionImpl>
    template <typename Msg>
    inline void ClientSessionBase<SessionImpl>::procoess_message_from_client(Msg &msg)
    {
        if (auto reject_info = enrich_message_from_client(msg); reject_info != true)
        {
            reject_client_message(msg, reject_info);
        }

        if (auto reject_info = send_message_from_client(msg); reject_info != true)
        {
            reject_client_message(msg, reject_info);
        }

        update_routing_info(msg);

        // persist client message
        // publish client message downstream
    }

    template <typename SessionImpl>
    template <typename Msg>
    inline RejectInfo ClientSessionBase<SessionImpl>::procoess_message_to_client(Msg &msg)
    {
        if (auto reject_info = enrich_message_to_client(msg); reject_info != true)
            return reject_info;

        if (auto reject_info = send_message_to_client(msg); reject_info != true)
            return reject_info;

        return RejectInfo{};
    }

    template <typename SessionImpl>
    template <typename Msg>
    inline RejectInfo ClientSessionBase<SessionImpl>::enrich_message_from_client(Msg &msg)
    {
        return enricher_.enrich_message_from_client(msg);
    }

    template <typename SessionImpl>
    template <typename Msg>
    inline RejectInfo ClientSessionBase<SessionImpl>::enrich_message_to_client(Msg &msg)
    {
        return enricher_.enrich_message_to_client(msg);
    }

    template <typename SessionImpl>
    template <typename Msg>
    inline RejectInfo ClientSessionBase<SessionImpl>::send_message_from_client(Msg &msg)
    {
        return std::visit([&msg](auto &&router)
                          { return std::forward<decltype(router)>(router).on_message_from_client(msg); },
                          forward_router_);
    }

    template <typename SessionImpl>
    template <typename Msg>
    inline RejectInfo ClientSessionBase<SessionImpl>::send_message_to_client(Msg &msg)
    {
        std::cout << "send_message_to_client:" << msg << std::endl;
        std::string_view data{reinterpret_cast<char *>(&msg), sizeof(msg)};
        return transport_.send_data(data);
    }

    template <typename SessionImpl>
    template <typename Msg>
    inline void ClientSessionBase<SessionImpl>::update_routing_info(Msg &msg)
    {
        // if constexpr (std::is_same_v<Msg, protocol_a::NewOrderSingle>)
        //{
        ClientSessionPtrVarient client_session_varient{&(this->impl())};
        reverse_router_.update_reverse_routing(123, client_session_varient);
        //}
    }
}