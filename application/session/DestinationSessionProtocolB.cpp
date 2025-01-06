#include <application/session/DestinationSessionProtocolB.h>

namespace max::protocol_b
{
    void DestinationSessionProtocolB::on_message_from_transport_impl(protocol_b::session::ExecutionReport &msg)
    {
        protocol_a::schema::ExecutionReport s_msg{};
        protocol_a::session::ExecutionReport src_msg{s_msg};
        procoess_message_from_transport(msg, src_msg);
    }

    void DestinationSessionProtocolB::on_message_from_transport_impl(protocol_b::session::CancelReject &msg)
    {
        protocol_a::schema::CancelReject s_msg{};
        protocol_a::session::CancelReject src_msg{s_msg};
        procoess_message_from_transport(msg, src_msg);
    }

    /* todox: move to codec */
    RejectInfo DestinationSessionProtocolB::decode_message_from_destination_impl(protocol_b::session::ExecutionReport &dst_msg,
                                                                                 protocol_a::session::ExecutionReport &src_msg)
    {
        std::cout << "decode_message_from_destination:" << dst_msg << std::endl;
        auto &msg = src_msg.msg();
        msg.a = static_cast<std::uint32_t>(dst_msg.msg().a);
        msg.b = static_cast<std::uint32_t>(dst_msg.msg().b);
        return RejectInfo{};
    }

    RejectInfo DestinationSessionProtocolB::decode_message_from_destination_impl(protocol_b::session::CancelReject &dst_msg,
                                                                                 protocol_a::session::CancelReject &src_msg)
    {
        std::cout << "decode_message_from_destination:" << dst_msg << std::endl;
        auto &msg = src_msg.msg();
        msg.a = static_cast<std::uint32_t>(dst_msg.msg().a);
        msg.b = static_cast<std::uint32_t>(dst_msg.msg().b);
        return RejectInfo{};
    }

    void DestinationSessionProtocolB::rejecet_message_from_transport_impl(protocol_b::session::ExecutionReport &msg, RejectInfo &reject_info)
    {
    }

    void DestinationSessionProtocolB::rejecet_message_from_transport_impl(protocol_b::session::CancelReject &msg, RejectInfo &reject_info)
    {
    }

    RejectInfo DestinationSessionProtocolB::on_message_from_peer_impl(protocol_a::session::NewOrderSingle &msg)
    {
        protocol_b::schema::NewOrderSingle s_msg{};
        protocol_b::session::NewOrderSingle dst_msg{s_msg};
        return procoess_message_to_transport(msg, dst_msg);
    }

    RejectInfo DestinationSessionProtocolB::on_message_from_peer_impl(protocol_a::session::CancelReplaceRequest &msg)
    {
        return RejectInfo();
    }

    RejectInfo DestinationSessionProtocolB::on_message_from_peer_impl(protocol_a::session::CancelRequest &msg)
    {
        return RejectInfo();
    }

    RejectInfo DestinationSessionProtocolB::encode_message_to_destination_impl(protocol_a::session::NewOrderSingle &src_msg,
                                                                               protocol_b::session::NewOrderSingle &dst_msg)
    {
        //std::cout << "encode_message_to_destination:" << src_msg << std::endl;
        auto &dest_msg = dst_msg.msg();
        dest_msg.a = src_msg.msg().a;
        dest_msg.b = static_cast<int>(src_msg.msg().b);
        //codec::encode(source,dest;)
        return RejectInfo{};
    }

    /* todox: move to codec */
    RejectInfo DestinationSessionProtocolB::encode_message_to_destination_impl(protocol_a::session::CancelReplaceRequest &src_msg,
                                                                               protocol_b::session::CancelReplaceRequest &dst_msg)
    {
        std::cout << "encode_message_to_destination:" << src_msg << std::endl;
        auto &dest_msg = dst_msg.msg();
        dest_msg.a = static_cast<int>(src_msg.msg().a);
        dest_msg.b = static_cast<int>(src_msg.msg().b);
        return RejectInfo{};
    }

    RejectInfo DestinationSessionProtocolB::encode_message_to_destination_impl(protocol_a::session::CancelRequest &src_msg,
                                                                               protocol_b::session::CancelRequest &dst_msg)
    {
        std::cout << "encode_message_to_destination:" << src_msg << std::endl;
        auto &dest_msg = dst_msg.msg();
        dest_msg.a = static_cast<int>(src_msg.msg().a);
        dest_msg.b = static_cast<int>(src_msg.msg().b);
        return RejectInfo{};
    }
}
