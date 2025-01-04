#pragma once

#include <application/enricher/ClientEnricher.h>
#include <application/router/ForwardRouters.h>
#include <application/router/ReverseRouter.h>
#include <application/session/ClientSession.h>
#include <application/session/RejectInfo.h>
#include <application/message/protocol_a/Messages.h>
#include <type_traits>
#include <iostream>

namespace max::protocol_a
{
    class ClientSessionProtocolA : public ClientSessionBase<ClientSessionProtocolA>
    {
    public:
        explicit ClientSessionProtocolA(ForwardRouterVarient &forward_router,
                                        ReverseRouter &reverse_router,
                                        Transport &transport)
            : ClientSessionBase<ClientSessionProtocolA>{forward_router, reverse_router, transport} {}

        /* TransportEvents */
        void on_connect_impl();
        void on_disconnect_impl();

        /* Messages from transport */
        template <typename Msg>
        void on_message_from_transport_impl(Msg &msg);
        void on_message_from_transport_impl(session::NewOrderSingle &msg);
        void on_message_from_transport_impl(session::CancelReplaceRequest &msg);
        void on_message_from_transport_impl(session::CancelRequest &msg);

        /* Internal Rejects */
        void rejecet_message_from_transport_impl(session::NewOrderSingle &msg, RejectInfo &reject_info);
        void rejecet_message_from_transport_impl(session::CancelReplaceRequest &msg, RejectInfo &reject_info);
        void rejecet_message_from_transport_impl(session::CancelRequest &msg, RejectInfo &reject_info);

        /* messages from peer session to transport */
        RejectInfo on_message_from_peer_impl(session::ExecutionReport &msg);
        RejectInfo on_message_from_peer_impl(session::CancelReject &msg);
    };

    template <typename Msg>
    inline void ClientSessionProtocolA::on_message_from_transport_impl(Msg &msg)
    {
        std::cout << "unknown client message" << std::endl;
    }
}