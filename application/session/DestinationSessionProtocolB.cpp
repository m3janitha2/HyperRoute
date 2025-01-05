#include <application/session/DestinationSessionProtocolB.h>

namespace max::protocol_b
{
    void DestinationSessionProtocolB::on_message_from_transport_impl(session::ExecutionReport &msg)
    {
        procoess_message_from_transport(msg);
    }

    void DestinationSessionProtocolB::on_message_from_transport_impl(session::CancelReject &msg)
    {
        procoess_message_from_transport(msg);
    }

    std::pair<RejectInfo, protocol_b::session::NewOrderSingle> DestinationSessionProtocolB::encode_message_to_destination_impl(protocol_a::session::NewOrderSingle &source_msg)
    {
        std::cout << "encode_message_to_destination:" << source_msg << std::endl;
        protocol_b::schema::NewOrderSingle destination_msg{};
        destination_msg.a = source_msg.msg().a;
        destination_msg.b = static_cast<int>(source_msg.msg().b);
        protocol_b::session::NewOrderSingle destination_msg_wrap{destination_msg};
        return std::pair{RejectInfo{}, destination_msg_wrap};
    }

    std::pair<RejectInfo, protocol_b::session::CancelReplaceRequest> DestinationSessionProtocolB::encode_message_to_destination_impl(protocol_a::session::CancelReplaceRequest &source_msg)
    {
        std::cout << "encode_message_to_destination:" << source_msg << std::endl;
        protocol_b::schema::CancelReplaceRequest destination_msg{};
        destination_msg.a = static_cast<int>(source_msg.msg().a);
        destination_msg.b = static_cast<int>(source_msg.msg().b);
        protocol_b::session::CancelReplaceRequest destination_msg_wrap{destination_msg};
        return std::pair{RejectInfo{}, destination_msg_wrap};
    }

    std::pair<RejectInfo, protocol_b::session::CancelRequest> DestinationSessionProtocolB::encode_message_to_destination_impl(protocol_a::session::CancelRequest &source_msg)
    {
        std::cout << "encode_message_to_destination:" << source_msg << std::endl;
        protocol_b::schema::CancelRequest destination_msg{};
        destination_msg.a = static_cast<int>(source_msg.msg().a);
        destination_msg.b = static_cast<int>(source_msg.msg().b);
        protocol_b::session::CancelRequest destination_msg_wrap{destination_msg};
        return std::pair{RejectInfo{}, destination_msg_wrap};
    }

    std::pair<RejectInfo, protocol_a::session::ExecutionReport> DestinationSessionProtocolB::decode_message_from_destination_impl(session::ExecutionReport &destination_msg)
    {
        std::cout << "decode_message_from_destination:" << destination_msg << std::endl;
        protocol_a::schema::ExecutionReport source_msg{};
        source_msg.a = static_cast<std::uint32_t>(destination_msg.msg().a);
        source_msg.b = static_cast<std::uint32_t>(destination_msg.msg().b);
        protocol_a::session::ExecutionReport source_msg_wrap{source_msg};
        return std::pair{RejectInfo{}, source_msg_wrap};
    }

    std::pair<RejectInfo, protocol_a::session::CancelReject> DestinationSessionProtocolB::decode_message_from_destination_impl(session::CancelReject &destination_msg)
    {
        std::cout << "decode_message_from_destination:" << destination_msg << std::endl;
        protocol_a::schema::CancelReject source_msg{};
        source_msg.a = static_cast<std::uint32_t>(destination_msg.msg().a);
        source_msg.b = static_cast<std::uint32_t>(destination_msg.msg().b);
        protocol_a::session::CancelReject source_msg_wrap{source_msg};
        return std::pair{RejectInfo{}, source_msg_wrap};
    }

    void DestinationSessionProtocolB::rejecet_message_from_transport_impl(session::ExecutionReport &msg, RejectInfo &reject_info)
    {
    }

    void DestinationSessionProtocolB::rejecet_message_from_transport_impl(session::CancelReject &msg, RejectInfo &reject_info)
    {
    }
}
