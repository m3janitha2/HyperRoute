#include <example/trading_system/session/DestinationSessionProtocolB.h>
#include "DestinationSessionProtocolB.h"

namespace hyper::protocol_b
{
    void DestinationSessionProtocolB::on_connect_impl() noexcept
    {
    }

    void DestinationSessionProtocolB::on_disconnect_impl() noexcept
    {
    }

    void DestinationSessionProtocolB::on_message_from_transport_impl(protocol_b::session::ExecutionReport &dest_msg)
    {
        protocol_a::schema::ExecutionReport msg{};
        protocol_a::session::ExecutionReport src_msg{msg};

        /* New Order Ack */
        if (auto it = src_routing_info_by_dest_cl_ord_id_.find(dest_msg.msg().c);
            it == src_routing_info_by_dest_cl_ord_id_.end()) [[unlikely]]
        {
            std::cout << "Source Cl ord Id Not Found:" << dest_msg << std::endl;
            return;
        }
        else
        {
            auto [cl_ord_id, uid] = it->second;
            src_msg.cl_ord_id(cl_ord_id);
            src_msg.uid(uid);
        }

        procoess_message_from_transport(dest_msg, src_msg);
    }

    void DestinationSessionProtocolB::on_message_from_transport_impl(protocol_b::session::CancelReject &dest_msg)
    {
        protocol_a::schema::CancelReject msg{};
        protocol_a::session::CancelReject src_msg{msg};
        procoess_message_from_transport(dest_msg, src_msg);
    }

    /* todox: move to codec */
    RejectInfo DestinationSessionProtocolB::decode_message_from_destination_impl(protocol_b::session::ExecutionReport &dst_msg,
                                                                                 protocol_a::session::ExecutionReport &src_msg)
    {
        // std::cout << "decode_message_from_destination:" << dst_msg << std::endl;
        auto &msg = src_msg.msg();
        msg.a = static_cast<std::uint32_t>(dst_msg.msg().a);
        msg.b = static_cast<std::uint32_t>(dst_msg.msg().b);
        return RejectInfo{};
    }

    RejectInfo DestinationSessionProtocolB::decode_message_from_destination_impl(protocol_b::session::CancelReject &dst_msg,
                                                                                 protocol_a::session::CancelReject &src_msg)
    {
        // std::cout << "decode_message_from_destination:" << dst_msg << std::endl;
        auto &msg = src_msg.msg();
        msg.a = static_cast<std::uint32_t>(dst_msg.msg().a);
        msg.b = static_cast<std::uint32_t>(dst_msg.msg().b);
        return RejectInfo{};
    }

    void DestinationSessionProtocolB::rejecet_message_from_transport_impl(protocol_b::session::ExecutionReport &msg, RejectInfo &reject_info)
    {
        std::cout << "message rejected. reason:" << reject_info << " msg:" << msg << std::endl;
    }

    void DestinationSessionProtocolB::rejecet_message_from_transport_impl(protocol_b::session::CancelReject &msg, RejectInfo &reject_info)
    {
        std::cout << "message rejected. reason:" << reject_info << " msg:" << msg << std::endl;
    }

    RejectInfo DestinationSessionProtocolB::on_message_from_peer_impl(protocol_a::session::NewOrderSingle &src_msg)
    {
        protocol_b::schema::NewOrderSingle msg{};
        protocol_b::session::NewOrderSingle dst_msg{msg};

        auto dest_cl_ord_id = destination_id_generator_.get_next_uid();
        msg.c = dest_cl_ord_id;
        return procoess_message_to_transport(src_msg, dst_msg);
    }

    RejectInfo DestinationSessionProtocolB::on_message_from_peer_impl(protocol_a::session::CancelReplaceRequest &src_msg)
    {
        protocol_b::schema::CancelReplaceRequest msg{};
        protocol_b::session::CancelReplaceRequest dst_msg{msg};

        auto dest_cl_ord_id = destination_id_generator_.get_next_uid();
        msg.c = dest_cl_ord_id;

        return procoess_message_to_transport(src_msg, dst_msg);
    }

    RejectInfo DestinationSessionProtocolB::on_message_from_peer_impl(protocol_a::session::CancelRequest &src_msg)
    {
        protocol_b::schema::CancelRequest msg{};
        protocol_b::session::CancelRequest dst_msg{msg};
        return procoess_message_to_transport(src_msg, dst_msg);
    }

    RejectInfo DestinationSessionProtocolB::encode_message_to_destination_impl(protocol_a::session::NewOrderSingle &src_msg,
                                                                               protocol_b::session::NewOrderSingle &dst_msg)
    {
        // std::cout << "encode_message_to_destination:" << src_msg << std::endl;
        auto &dest_msg = dst_msg.msg();
        dest_msg.a = src_msg.msg().a;
        dest_msg.b = static_cast<int>(src_msg.msg().b);
        // codec::encode(source,dest;)
        return RejectInfo{};
    }

    /* todox: move to codec */
    RejectInfo DestinationSessionProtocolB::encode_message_to_destination_impl(protocol_a::session::CancelReplaceRequest &src_msg,
                                                                               protocol_b::session::CancelReplaceRequest &dst_msg)
    {
        // std::cout << "encode_message_to_destination:" << src_msg << std::endl;
        auto &dest_msg = dst_msg.msg();
        dest_msg.a = static_cast<int>(src_msg.msg().a);
        dest_msg.b = static_cast<int>(src_msg.msg().b);
        return RejectInfo{};
    }

    RejectInfo DestinationSessionProtocolB::encode_message_to_destination_impl(protocol_a::session::CancelRequest &src_msg,
                                                                               protocol_b::session::CancelRequest &dst_msg)
    {
        // std::cout << "encode_message_to_destination:" << src_msg << std::endl;
        auto &dest_msg = dst_msg.msg();
        dest_msg.a = static_cast<int>(src_msg.msg().a);
        dest_msg.b = static_cast<int>(src_msg.msg().b);
        return RejectInfo{};
    }

    inline void DestinationSessionProtocolB::update_destination_routing_info_impl(protocol_a::session::NewOrderSingle &src_msg,
                                                                                  protocol_b::session::NewOrderSingle &dst_msg) noexcept
    {
        src_routing_info_by_dest_cl_ord_id_.emplace(std::piecewise_construct,
                                                    std::forward_as_tuple(dst_msg.msg().c),
                                                    std::forward_as_tuple(src_msg.cl_ord_id(), src_msg.uid()));
        dest_cl_ord_id_by_src_cl_ord_id_.emplace(src_msg.cl_ord_id(), dst_msg.msg().c);
    }

    void DestinationSessionProtocolB::update_destination_routing_info_impl([[maybe_unused]] protocol_a::session::CancelReplaceRequest &src_msg,
                                                                           [[maybe_unused]] protocol_b::session::CancelReplaceRequest &dst_msg) noexcept
    {
    }

    void DestinationSessionProtocolB::update_destination_routing_info_impl([[maybe_unused]] protocol_a::session::CancelRequest &src_msg,
                                                                           [[maybe_unused]] protocol_b::session::CancelRequest &dst_msg) noexcept
    {
    }
}
