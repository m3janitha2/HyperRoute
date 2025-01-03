#include <application/session/VenueSessionProtocolB.h>

namespace max::protocol_b
{
    void VenueSessionProtocolB::on_message_impl(session::ExecutionReport &msg)
    {
        procoess_message_from_venue(msg);
    }

    void VenueSessionProtocolB::on_message_impl(session::CancelReject &msg)
    {
        procoess_message_from_venue(msg);
    }

    std::pair<RejectInfo, protocol_b::session::NewOrderSingle> VenueSessionProtocolB::encode_message_to_venue_impl(protocol_a::session::NewOrderSingle &client_msg)
    {
        std::cout << "encode_message_to_venue:" << client_msg << std::endl;
        protocol_b::schema::NewOrderSingle venue_msg{};
        venue_msg.a = client_msg.msg().a;
        venue_msg.b = static_cast<int>(client_msg.msg().b);
        protocol_b::session::NewOrderSingle venue_msg_wrap{venue_msg};
        return std::pair{RejectInfo{}, venue_msg_wrap};
    }

    std::pair<RejectInfo, protocol_b::session::CancelReplaceRequest> VenueSessionProtocolB::encode_message_to_venue_impl(protocol_a::session::CancelReplaceRequest &client_msg)
    {
        std::cout << "encode_message_to_venue:" << client_msg << std::endl;
        protocol_b::schema::CancelReplaceRequest venue_msg{};
        venue_msg.a = static_cast<int>(client_msg.msg().a);
        venue_msg.b = static_cast<int>(client_msg.msg().b);
        protocol_b::session::CancelReplaceRequest venue_msg_wrap{venue_msg};
        return std::pair{RejectInfo{}, venue_msg_wrap};
    }

    std::pair<RejectInfo, protocol_b::session::CancelRequest> VenueSessionProtocolB::encode_message_to_venue_impl(protocol_a::session::CancelRequest &client_msg)
    {
        std::cout << "encode_message_to_venue:" << client_msg << std::endl;
        protocol_b::schema::CancelRequest venue_msg{};
        venue_msg.a = static_cast<int>(client_msg.msg().a);
        venue_msg.b = static_cast<int>(client_msg.msg().b);
        protocol_b::session::CancelRequest venue_msg_wrap{venue_msg};
        return std::pair{RejectInfo{}, venue_msg_wrap};
    }

    std::pair<RejectInfo, protocol_a::session::ExecutionReport> VenueSessionProtocolB::decode_message_from_venue_impl(session::ExecutionReport &venue_msg)
    {
        std::cout << "decode_message_from_venue:" << venue_msg << std::endl;
        protocol_a::schema::ExecutionReport client_msg{};
        client_msg.a = static_cast<std::uint32_t>(venue_msg.msg().a);
        client_msg.b = static_cast<std::uint32_t>(venue_msg.msg().b);
        protocol_a::session::ExecutionReport client_msg_wrap{client_msg};
        return std::pair{RejectInfo{}, client_msg_wrap};
    }

    std::pair<RejectInfo, protocol_a::session::CancelReject> VenueSessionProtocolB::decode_message_from_venue_impl(session::CancelReject &venue_msg)
    {
        std::cout << "decode_message_from_venue:" << venue_msg << std::endl;
        protocol_a::schema::CancelReject client_msg{};
        client_msg.a = static_cast<std::uint32_t>(venue_msg.msg().a);
        client_msg.b = static_cast<std::uint32_t>(venue_msg.msg().b);
        protocol_a::session::CancelReject client_msg_wrap{client_msg};
        return std::pair{RejectInfo{}, client_msg_wrap};
    }

    void VenueSessionProtocolB::reject_venue_message_impl(session::ExecutionReport &msg, RejectInfo &reject_info)
    {
    }

    void VenueSessionProtocolB::reject_venue_message_impl(session::CancelReject &msg, RejectInfo &reject_info)
    {
    }
}
