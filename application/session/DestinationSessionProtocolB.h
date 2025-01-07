#pragma once

#include <framework/protocol/DestinationSession.h>
#include <application/message/protocol_b/Messages.h>
#include <application/message/protocol_a/Messages.h>

#include <iostream>

namespace hyper::protocol_b
{
    using RejectInfo = framework::RejectInfo;
    using SourceRouter = framework::SourceRouter;
    using Transport = framework::Transport;

    class DestinationSessionProtocolB : public framework::DestinationSession<DestinationSessionProtocolB>
    {
    public:
        explicit DestinationSessionProtocolB(SourceRouter &source_router,
                                             Transport &transport)
            : DestinationSession<DestinationSessionProtocolB>{source_router, transport} {}

        /* TransportEvents */
        void on_connect_impl();
        void on_disconnect_impl();

        /* Messages from the source to the destination */
        void on_message_from_transport_impl(session::ExecutionReport &msg);
        void on_message_from_transport_impl(session::CancelReject &msg);

        RejectInfo decode_message_from_destination_impl(protocol_b::session::ExecutionReport &dst_msg,
                                                        protocol_a::session::ExecutionReport &src_msg);
        RejectInfo decode_message_from_destination_impl(protocol_b::session::CancelReject &dst_msg,
                                                        protocol_a::session::CancelReject &src_msg);

        void rejecet_message_from_transport_impl(protocol_b::session::ExecutionReport &msg, RejectInfo &reject_info);
        void rejecet_message_from_transport_impl(protocol_b::session::CancelReject &msg, RejectInfo &reject_info);

        /* Messages from the destination to the source */
        RejectInfo on_message_from_peer_impl(protocol_a::session::NewOrderSingle &msg);
        RejectInfo on_message_from_peer_impl(protocol_a::session::CancelReplaceRequest &msg);
        RejectInfo on_message_from_peer_impl(protocol_a::session::CancelRequest &msg);

        template <typename SourceMsg, typename DestinationMsg>
        RejectInfo encode_message_to_destination_impl(SourceMsg &src_msg,
                                                      DestinationMsg &dst_msg);
        RejectInfo encode_message_to_destination_impl(protocol_a::session::NewOrderSingle &src_msg,
                                                      protocol_b::session::NewOrderSingle &dst_msg);
        RejectInfo encode_message_to_destination_impl(protocol_a::session::CancelReplaceRequest &src_msg,
                                                      protocol_b::session::CancelReplaceRequest &dst_msg);
        RejectInfo encode_message_to_destination_impl(protocol_a::session::CancelRequest &src_msg,
                                                      protocol_b::session::CancelRequest &dst_msg);
    };

    template <typename SourceMsg, typename DestinationMsg>
    inline RejectInfo DestinationSessionProtocolB::encode_message_to_destination_impl(SourceMsg &src_msg, DestinationMsg &dst_msg)
    {
        std::cout << "unknown message" << std::endl;
        return RejectInfo{};
    }
}