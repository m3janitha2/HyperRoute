#pragma once

#include <application/message/protocol_b/Messages.h>
#include <application/message/protocol_a/Messages.h>
#include <application/enricher/VenueEnricher.h>
#include <application/validator/Validator.h>
#include <application/router/ReverseRouter.h>
#include <application/session/VenueSession.h>
#include <iostream>

namespace max::protocol_b
{
    class VenueSessionProtocolB : public VenueSessionBase<VenueSessionProtocolB>
    {
    public:
        explicit VenueSessionProtocolB(ReverseRouter &reverse_router,
                                       Transport &transport)
            : VenueSessionBase<VenueSessionProtocolB>{reverse_router, transport} {}

        /* TransportEvents */
        void on_connect_impl();
        void on_disconnect_impl();

        /* messages from venue to client */
        void on_message_impl(session::ExecutionReport &msg);
        void on_message_impl(session::CancelReject &msg);

        template <typename Msg>
        auto encode_message_to_venue_impl(Msg &msg);
        std::pair<RejectInfo, protocol_b::session::NewOrderSingle> encode_message_to_venue_impl(protocol_a::session::NewOrderSingle &msg);
        std::pair<RejectInfo, protocol_b::session::CancelReplaceRequest> encode_message_to_venue_impl(protocol_a::session::CancelReplaceRequest &msg);
        std::pair<RejectInfo, protocol_b::session::CancelRequest> encode_message_to_venue_impl(protocol_a::session::CancelRequest &msg);

        std::pair<RejectInfo, protocol_a::session::ExecutionReport> decode_message_from_venue_impl(session::ExecutionReport &msg);
        std::pair<RejectInfo, protocol_a::session::CancelReject> decode_message_from_venue_impl(session::CancelReject &msg);

        void reject_venue_message_impl(session::ExecutionReport &msg, RejectInfo &reject_info);
        void reject_venue_message_impl(session::CancelReject &msg, RejectInfo &reject_info);

    private:
    };

    template <typename Msg>
    inline auto VenueSessionProtocolB::encode_message_to_venue_impl(Msg &msg)
    {
        std::cout << "unknown message" << std::endl;
        protocol_b::session::NewOrderSingle venue_msg{};
        return std::pair{RejectInfo{"Protocol B encoding unknown message", InteranlRejectCode::To_Venue_Encoding_Failed}, venue_msg};
    }
}