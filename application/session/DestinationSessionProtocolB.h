#pragma once

#include <application/message/protocol_b/Messages.h>
#include <application/message/protocol_a/Messages.h>
#include <application/enricher/VenueEnricher.h>
#include <application/validator/Validator.h>
#include <application/router/ReverseRouter.h>
#include <application/session/DestinationSession.h>
#include <iostream>

namespace max::protocol_b
{
    class DestinationSessionProtocolB : public DestinationSessionBase<DestinationSessionProtocolB>
    {
    public:
        explicit DestinationSessionProtocolB(ReverseRouter &reverse_router,
                                             Transport &transport)
            : DestinationSessionBase<DestinationSessionProtocolB>{reverse_router, transport} {}

        /* TransportEvents */
        void on_connect_impl();
        void on_disconnect_impl();

        /* Messages from the source to the destination */        
        void on_message_from_transport_impl(session::ExecutionReport &msg);
        void on_message_from_transport_impl(session::CancelReject &msg);

        RejectInfo decode_message_from_destination_impl(protocol_b::session::ExecutionReport &destination_msg,
                                                        protocol_a::session::ExecutionReport &source_msg);
        RejectInfo decode_message_from_destination_impl(protocol_b::session::CancelReject &destination_msg,
                                                        protocol_a::session::CancelReject &source_msg);

        void rejecet_message_from_transport_impl(protocol_b::session::ExecutionReport &msg, RejectInfo &reject_info);
        void rejecet_message_from_transport_impl(protocol_b::session::CancelReject &msg, RejectInfo &reject_info);

        /* Messages from the destination to the source */
        RejectInfo on_message_from_peer_impl(protocol_a::session::NewOrderSingle &msg);
        RejectInfo on_message_from_peer_impl(protocol_a::session::CancelReplaceRequest &msg);
        RejectInfo on_message_from_peer_impl(protocol_a::session::CancelRequest &msg);

        template <typename SourceMsg, typename DestinationMsg>
        RejectInfo encode_message_to_destination_impl(SourceMsg &source_msg,
                                                DestinationMsg &destination_msg);
        RejectInfo encode_message_to_destination_impl(protocol_a::session::NewOrderSingle &source_msg,
                                                      protocol_b::session::NewOrderSingle &destination_msg);
        RejectInfo encode_message_to_destination_impl(protocol_a::session::CancelReplaceRequest &source_msg,
                                                      protocol_b::session::CancelReplaceRequest &destination_msg);
        RejectInfo encode_message_to_destination_impl(protocol_a::session::CancelRequest &source_msg,
                                                      protocol_b::session::CancelRequest &destination_msg);
    };

    template <typename SourceMsg, typename DestinationMsg>
    inline RejectInfo DestinationSessionProtocolB::encode_message_to_destination_impl(SourceMsg &source_msg, DestinationMsg &destination_msg)
    {
        std::cout << "unknown message" << std::endl;
        return RejectInfo{};
    }
}