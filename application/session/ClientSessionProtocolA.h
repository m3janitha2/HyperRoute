#pragma once

#include <application/enricher/ClientEnricher.h>
#include <application/router/ForwardRouters.h>
#include <application/router/ReverseRouter.h>
#include <application/session/ClientSession.h>
#include <application/session/RejectInfo.h>
#include <application/message/protocol_a/NewOrderSingle.h>
#include <type_traits>
namespace max
{
    class ClientSessionProtocolA : public ClientSessionBase<ClientSessionProtocolA>
    {
    public:
        explicit ClientSessionProtocolA(ForwardRouterVarient &forward_router, ReverseRouter &reverse_router)
            : ClientSessionBase<ClientSessionProtocolA>(forward_router,reverse_router) {}

        /* session level events */
        void on_data_impl();
        void on_connect_impl();
        void on_disconnect_impl();

        /* messages from client */
        void on_client_message_impl(protocol_a::NewOrderSingle &msg);
        void on_client_message_impl(protocol_a::CancelReplaceRequest &msg);
        void on_client_message_impl(protocol_a::CancelRequest &msg);

        /* messages to client */
        RejectInfo on_venue_message_impl(protocol_a::ExecutionReport &msg);
        RejectInfo on_venue_message_impl(protocol_a::CancelReject &msg);

        /* handle internal message rejects */
        void reject_client_message_impl(protocol_a::NewOrderSingle &msg, RejectInfo &reject_info);
        void reject_client_message_impl(protocol_a::CancelReplaceRequest &msg, RejectInfo &reject_info);
        void reject_client_message_impl(protocol_a::CancelRequest &msg, RejectInfo &reject_info);
    };
}