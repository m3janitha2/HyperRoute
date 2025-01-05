#include <application/session/DestinationSessionProtocolB.h>
#include "DestinationSessionProtocolB.h"

namespace max::protocol_b
{
    void DestinationSessionProtocolB::on_message_from_transport_impl(protocol_b::session::ExecutionReport &msg)
    {
        protocol_a::session::ExecutionReport source_msg{};
        procoess_message_from_transport(msg, source_msg);
    }

    void DestinationSessionProtocolB::on_message_from_transport_impl(protocol_b::session::CancelReject &msg)
    {
        protocol_a::session::CancelReject source_msg{};
        procoess_message_from_transport(msg, source_msg);
    }

    /* todox: move to codec */
    RejectInfo DestinationSessionProtocolB::decode_message_from_destination_impl(protocol_b::session::ExecutionReport &destination_msg,
                                                                                 protocol_a::session::ExecutionReport &source_msg)
    {
        std::cout << "decode_message_from_destination:" << destination_msg << std::endl;
        auto &src_msg = source_msg.msg();
        src_msg.a = static_cast<std::uint32_t>(destination_msg.msg().a);
        src_msg.b = static_cast<std::uint32_t>(destination_msg.msg().b);
        return RejectInfo{};
    }

    RejectInfo DestinationSessionProtocolB::decode_message_from_destination_impl(protocol_b::session::CancelReject &destination_msg,
                                                                                 protocol_a::session::CancelReject &source_msg)
    {
        std::cout << "decode_message_from_destination:" << destination_msg << std::endl;
        auto &src_msg = source_msg.msg();
        src_msg.a = static_cast<std::uint32_t>(destination_msg.msg().a);
        src_msg.b = static_cast<std::uint32_t>(destination_msg.msg().b);
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
        protocol_b::session::NewOrderSingle destination_msg{};
        return procoess_message_to_transport(msg, destination_msg);
    }

    RejectInfo DestinationSessionProtocolB::on_message_from_peer_impl(protocol_a::session::CancelReplaceRequest &msg)
    {
        return RejectInfo();
    }

    RejectInfo DestinationSessionProtocolB::on_message_from_peer_impl(protocol_a::session::CancelRequest &msg)
    {
        return RejectInfo();
    }

    RejectInfo DestinationSessionProtocolB::encode_message_to_destination_impl(protocol_a::session::NewOrderSingle &source_msg,
                                                                               protocol_b::session::NewOrderSingle &destination_msg)
    {
        std::cout << "encode_message_to_destination:" << source_msg << std::endl;
        auto &dest_msg = destination_msg.msg();
        dest_msg.a = source_msg.msg().a;
        dest_msg.b = static_cast<int>(source_msg.msg().b);
        return RejectInfo{};
    }

    /* todox: move to codec */
    RejectInfo DestinationSessionProtocolB::encode_message_to_destination_impl(protocol_a::session::CancelReplaceRequest &source_msg,
                                                                               protocol_b::session::CancelReplaceRequest &destination_msg)
    {
        std::cout << "encode_message_to_destination:" << source_msg << std::endl;
        auto &dest_msg = destination_msg.msg();
        dest_msg.a = static_cast<int>(source_msg.msg().a);
        dest_msg.b = static_cast<int>(source_msg.msg().b);
        return RejectInfo{};
    }

    RejectInfo DestinationSessionProtocolB::encode_message_to_destination_impl(protocol_a::session::CancelRequest &source_msg,
                                                                               protocol_b::session::CancelRequest &destination_msg)
    {
        std::cout << "encode_message_to_destination:" << source_msg << std::endl;
        auto &dest_msg = destination_msg.msg();
        dest_msg.a = static_cast<int>(source_msg.msg().a);
        dest_msg.b = static_cast<int>(source_msg.msg().b);
        return RejectInfo{};
    }
}
