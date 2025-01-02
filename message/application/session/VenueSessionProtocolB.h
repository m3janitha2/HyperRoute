#pragma once
#include <iostream>

#include <application/message/protocol_b/NewOrderSingle.h>
#include <application/message/protocol_a/NewOrderSingle.h>
#include <application/enricher/VenueEnricher.h>
#include <application/validator/Validator.h>
#include <application/router/ReverseRouter.h>

namespace max
{
    class VenueSessionProtocolB
    {
    public:
        explicit VenueSessionProtocolB(ReverseRouter &reverse_router)
            : reverse_router_(reverse_router) {}
        /* session level events */
        void on_data();
        void on_connect();
        void on_disconnect();

        /* messages to venue */
        template <typename Msg>
        RejectInfo on_client_message(Msg &msg);

        /* messages from venue */
        void on_venue_message(protocol_b::ExecutionReport &msg);
        void on_venue_message(protocol_b::CancelReject &msg);

        /* venue session internal */
        template <typename Msg>
        RejectInfo procoess_message_to_venue(Msg &msg);
        template <typename Msg>
        void procoess_message_from_venue(Msg &msg);
        template <typename Msg>
        RejectInfo enrich_message_to_venue(Msg &msg);
        template <typename Msg>
        RejectInfo enrich_message_from_venue(Msg &msg);
        template <typename ClientMsg>
        auto encode_message_to_venue(ClientMsg &msg)
        {
            std::cout << "unknown message" << std::endl;
            protocol_b::NewOrderSingle venue_msg{};
            return std::pair{RejectInfo{"Protocol B encoding unknown message", InteranlRejectCode::To_Venue_Encoding_Failed}, venue_msg};
        }

        template <typename VenueMsg>
        auto decode_message_from_venue(VenueMsg &msg)
        {
            std::cout << "unknown message" << std::endl;
            protocol_b::ExecutionReport venue_msg{};
            return std::pair{RejectInfo{"Protocol B decoding unknown message", InteranlRejectCode::From_Venue_Decoding_Failed}, venue_msg};
        }

        template <typename Msg>
        RejectInfo send_message_to_venue(Msg &msg);
        template <typename Msg>
        RejectInfo send_message_to_client(Msg &msg);

        template <typename Msg>
        void reject_venue_message(Msg &msg, RejectInfo &reject_info);

    private:
        Validator validator_{};
        VenueEnricher venue_enricher_{};
        ReverseRouter &reverse_router_;
    };

    template <typename Msg>
    RejectInfo VenueSessionProtocolB::on_client_message(Msg &msg)
    {
        return procoess_message_to_venue(msg);
    }

    template <>
    inline auto VenueSessionProtocolB::encode_message_to_venue<protocol_a::NewOrderSingle>(protocol_a::NewOrderSingle &client_msg)
    {
        std::cout << "encode_message_to_venue:" << client_msg << std::endl;
        protocol_b::NewOrderSingle venue_msg{client_msg.a, client_msg.b};
        return std::pair{RejectInfo{}, venue_msg};
    }

    template <>
    inline auto VenueSessionProtocolB::encode_message_to_venue<protocol_a::CancelReplaceRequest>(protocol_a::CancelReplaceRequest &client_msg)
    {
        std::cout << "encode_message_to_venue:" << client_msg << std::endl;
        protocol_b::CancelReplaceRequest venue_msg{client_msg.a, client_msg.b};
        return std::pair{RejectInfo{}, venue_msg};
    }

    template <>
    inline auto VenueSessionProtocolB::encode_message_to_venue<protocol_a::CancelRequest>(protocol_a::CancelRequest &client_msg)
    {
        std::cout << "encode_message_to_venue:" << client_msg << std::endl;
        protocol_b::CancelRequest venue_msg{client_msg.a, client_msg.b};
        return std::pair{RejectInfo{}, venue_msg};
    }

    template <>
    inline auto VenueSessionProtocolB::decode_message_from_venue<protocol_b::ExecutionReport>(protocol_b::ExecutionReport &venue_msg)
    {
        std::cout << "decode_message_from_venue:" << venue_msg << std::endl;
        protocol_a::ExecutionReport client_msg{venue_msg.a, venue_msg.b};
        return std::pair{RejectInfo{}, client_msg};
    }

    template <>
    inline auto VenueSessionProtocolB::decode_message_from_venue<protocol_b::CancelReject>(protocol_b::CancelReject &venue_msg)
    {
        std::cout << "decode_message_from_venue:" << venue_msg << std::endl;
        protocol_a::CancelReject client_msg{venue_msg.a, venue_msg.b};
        return std::pair{RejectInfo{}, client_msg};
    }

    template <typename Msg>
    RejectInfo VenueSessionProtocolB::procoess_message_to_venue(Msg &client_msg)
    {
        if (auto reject_info = validator_.validate(client_msg); reject_info != true)
            return reject_info;

        auto [reject, venue_msg] = encode_message_to_venue(client_msg);
        if (reject != true)
            return reject;

        if (auto reject_info = enrich_message_to_venue(venue_msg); reject_info != true)
            return reject_info;

        if (auto reject_info = send_message_to_venue(venue_msg); reject_info != true)
            return reject_info;

        // persist venue message
        // publish venue message for downstream
        return RejectInfo{};
    }

    template <typename Msg>
    void VenueSessionProtocolB::procoess_message_from_venue(Msg &venue_msg)
    {
        if (auto reject_info = enrich_message_from_venue(venue_msg); reject_info != true)
            reject_venue_message(venue_msg, reject_info);

        auto [reject, client_msg] = decode_message_from_venue(venue_msg);
        if (reject != true)
            reject_venue_message(venue_msg, reject);

        if (auto reject_info = validator_.validate(client_msg); reject_info != true)
            reject_venue_message(venue_msg, reject_info);

        if (auto reject_info = send_message_to_client(client_msg); reject_info != true)
            reject_venue_message(venue_msg, reject_info);

        // persist venue message
        // publish venue message for downstream
    }

    template <typename Msg>
    RejectInfo VenueSessionProtocolB::enrich_message_to_venue(Msg &msg)
    {
        std::cout << "enrich_message_to_venue:" << msg << std::endl;
        return venue_enricher_.enrich_message_to_venue(msg);
    }

    template <typename Msg>
    RejectInfo VenueSessionProtocolB::enrich_message_from_venue(Msg &msg)
    {
        return venue_enricher_.enrich_message_from_venue(msg);
    }

    template <typename Msg>
    RejectInfo VenueSessionProtocolB::send_message_to_venue(Msg &msg)
    {
        std::cout << "send_message_to_venue:" << msg << std::endl;
        return RejectInfo{};
    }

    template <typename Msg>
    RejectInfo VenueSessionProtocolB::send_message_to_client(Msg &msg)
    {
        return reverse_router_.send_message_to_client(msg);
    }

    template <typename Msg>
    void VenueSessionProtocolB::reject_venue_message(Msg &msg, RejectInfo &reject_info)
    {
        std::cout << "reject_venue_message: " << msg << " reject info: " << reject_info << std::endl;
    }
}