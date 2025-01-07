#pragma once

#include <framework/protocol/SourceSession.h>
#include <application/message/protocol_a/Messages.h>
#include <type_traits>
#include <iostream>

namespace hyper::protocol_a
{
    using RejectInfo = framework::RejectInfo;
    using SourceRouter = framework::SourceRouter;
    using Transport = framework::Transport;
    using UID = framework::UID;

    class SourceSessionProtocolA : public framework::SourceSession<SourceSessionProtocolA>
    {
    public:
        explicit SourceSessionProtocolA(DestinationRouterPtrVarient &destination_router,
                                        SourceRouter &source_router,
                                        Transport &transport)
            : SourceSession<SourceSessionProtocolA>{destination_router, source_router, transport} {}

        /* TransportEvents */
        void on_connect_impl();
        void on_disconnect_impl();

        /* Messages from transport */
        template <typename Msg>
        void on_message_from_transport_impl(Msg &msg);
        void on_message_from_transport_impl(session::NewOrderSingle &msg);
        void on_message_from_transport_impl(session::CancelReplaceRequest &msg);
        void on_message_from_transport_impl(session::CancelRequest &msg);

        /* Internal rejects */
        void rejecet_message_from_transport_impl(session::NewOrderSingle &msg, RejectInfo &reject_info);
        void rejecet_message_from_transport_impl(session::CancelReplaceRequest &msg, RejectInfo &reject_info);
        void rejecet_message_from_transport_impl(session::CancelRequest &msg, RejectInfo &reject_info);

        template <typename Msg>
        void update_routing_info_impl(Msg &msg);

        /* Messages from the peer session to the transport */
        RejectInfo on_message_from_peer_impl(session::ExecutionReport &msg);
        RejectInfo on_message_from_peer_impl(session::CancelReject &msg);

    private:
        using ClOrdIdType = decltype(schema::NewOrderSingle::cl_ord_id);
        using ClOrdIDToUIDMap = std::unordered_map<ClOrdIdType, UID>;
        ClOrdIDToUIDMap cl_ord_id_to_uid_{};
    };

    template <typename Msg>
    inline void SourceSessionProtocolA::on_message_from_transport_impl(Msg &msg)
    {
        std::cout << "unknown source message" << std::endl;
    }

    template <typename Msg>
    inline void SourceSessionProtocolA::update_routing_info_impl(Msg &msg)
    {
        if constexpr (std::is_same_v<Msg, session::NewOrderSingle>)
        {
            cl_ord_id_to_uid_.emplace(msg.msg().cl_ord_id, msg.uid());
        }
    }
}