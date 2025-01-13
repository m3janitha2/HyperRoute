#pragma once

#include <framework/session/DestinationSession.h>
#include <framework/message/UIDGenerator.h>
#include <example/trading_system/message/protocol_b/Messages.h>
#include <example/trading_system/message/protocol_a/Messages.h>
#include <example/trading_system/utility/utility.h>

#include <iostream>

namespace hyper::protocol_b
{
    using RejectInfo = framework::RejectInfo;
    using InteranlRejectCode = framework::InteranlRejectCode;
    using SourceRouter = framework::SourceRouter;
    using Transport = framework::Transport;

    class DestinationSessionProtocolB : public framework::DestinationSession<DestinationSessionProtocolB>
    {
    public:
        explicit DestinationSessionProtocolB(Transport &transport,
                                             const SourceRouter &source_router,
                                             const ValidatorPtrVarient &validator)
            : DestinationSession<DestinationSessionProtocolB>{transport, source_router, validator} {}

        /* TransportEvents */
        void on_connect_impl() noexcept;
        void on_disconnect_impl() noexcept;

        /* Messages from the source to the destination */
        void on_message_from_transport_impl(session::ExecutionReport &msg) noexcept;
        void on_message_from_transport_impl(session::CancelReject &msg) noexcept;

        RejectInfo handle_message_from_transport_impl(protocol_b::session::ExecutionReport &dst_msg,
                                                      protocol_a::session::ExecutionReport &src_msg) noexcept;
        RejectInfo handle_message_from_transport_impl(protocol_b::session::CancelReject &dst_msg,
                                                      protocol_a::session::CancelReject &src_msg) noexcept;

        /* todox: move to codec */
        RejectInfo decode_message_from_destination_impl(protocol_b::session::ExecutionReport &dst_msg,
                                                        protocol_a::session::ExecutionReport &src_msg) noexcept;
        RejectInfo decode_message_from_destination_impl(protocol_b::session::CancelReject &dst_msg,
                                                        protocol_a::session::CancelReject &src_msg) noexcept;

        void reject_message_from_transport_impl(protocol_b::session::ExecutionReport &msg,
                                                RejectInfo &reject_info) noexcept;
        void reject_message_from_transport_impl(protocol_b::session::CancelReject &msg,
                                                RejectInfo &reject_info) noexcept;

        /* Messages from the destination to the source */
        RejectInfo on_message_from_peer_impl(protocol_a::session::NewOrderSingle &msg) noexcept;
        RejectInfo on_message_from_peer_impl(protocol_a::session::CancelReplaceRequest &msg) noexcept;
        RejectInfo on_message_from_peer_impl(protocol_a::session::CancelRequest &msg) noexcept;

        /* todox: move to codec */
        template <typename SourceMsg, typename DestinationMsg>
        RejectInfo encode_message_to_destination_impl(SourceMsg &src_msg,
                                                      DestinationMsg &dst_msg) noexcept;
        RejectInfo encode_message_to_destination_impl(protocol_a::session::NewOrderSingle &src_msg,
                                                      protocol_b::session::NewOrderSingle &dst_msg) noexcept;
        RejectInfo encode_message_to_destination_impl(protocol_a::session::CancelReplaceRequest &src_msg,
                                                      protocol_b::session::CancelReplaceRequest &dst_msg) noexcept;
        RejectInfo encode_message_to_destination_impl(protocol_a::session::CancelRequest &src_msg,
                                                      protocol_b::session::CancelRequest &dst_msg) noexcept;

        void update_destination_routing_info_impl(protocol_a::session::NewOrderSingle &src_msg,
                                                  protocol_b::session::NewOrderSingle &dst_msg) noexcept;
        void update_destination_routing_info_impl(protocol_a::session::CancelReplaceRequest &src_msg,
                                                  protocol_b::session::CancelReplaceRequest &dst_msg) noexcept;
        void update_destination_routing_info_impl(protocol_a::session::CancelRequest &src_msg,
                                                  protocol_b::session::CancelRequest &dst_msg) noexcept;

    private:
        template <typename DestMsg>
        void handle_cl_ord_id_to_destination(DestMsg &dst_msg);
        template <typename SrcMsg, typename DestMsg>
        RejectInfo handle_orig_cl_ord_id_to_destination(SrcMsg &src_msg, DestMsg &dst_msg);
        template <typename DestMsg, typename SrcMsg>
        RejectInfo handle_cl_ord_id_from_destination(DestMsg &dst_msg, SrcMsg &src_msg);

        using SrcClOrdIDType = std::uint64_t;
        using DestClOrdIDType = std::uint64_t;
        /* Consider using a Boost Multi-Index Container */
        using DestClOrdIDBySrcClOrdIDType = std::unordered_map<SrcClOrdIDType, DestClOrdIDType>;
        struct SourceRotingInfo
        {
            SrcClOrdIDType cl_ord_id_{};
            framework::UID uid_;
        };
        using SrcRoutingInfoByDestClOrdIDType = std::unordered_map<DestClOrdIDType, SourceRotingInfo>;
        DestClOrdIDBySrcClOrdIDType dest_cl_ord_id_by_src_cl_ord_id_{optimal_order_count};
        SrcRoutingInfoByDestClOrdIDType src_routing_info_by_dest_cl_ord_id_{optimal_order_count};

        using DestinationIDGenarator = framework::UIDGenerator;
        DestinationIDGenarator &destination_id_generator_{DestinationIDGenarator::instance()};
    };

    template <typename SourceMsg, typename DestinationMsg>
    inline RejectInfo DestinationSessionProtocolB::encode_message_to_destination_impl([[maybe_unused]] SourceMsg &src_msg,
                                                                                      [[maybe_unused]] DestinationMsg &dst_msg) noexcept
    {
        std::cout << "unknown message" << std::endl;
        return RejectInfo{};
    }
}