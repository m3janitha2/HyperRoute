#pragma once

#include <framework/session/SourceSession.h>
#include <example/trading_system/message/protocol_a/Messages.h>
#include <example/trading_system/utility/utility.h>
#include <type_traits>
#include <iostream>

namespace hyper::protocol_a
{
    using RejectInfo = framework::RejectInfo;
    using SourceRouter = framework::SourceRouter;
    using Transport = framework::Transport;
    using UID = framework::UID;

    class SourceSessionProtocolA : public framework::SourceSession<SourceSessionProtocolA>
    {
    public:
        explicit SourceSessionProtocolA(Transport &transport,
                                        const DestinationRouterPtrVarient &destination_router,
                                        const SourceRouter &source_router)
            : SourceSession<SourceSessionProtocolA>{transport, destination_router, source_router} {}

        /* TransportEvents */
        void on_connect_impl() noexcept;
        void on_disconnect_impl() noexcept;

        /* Messages from transport */
        template <typename Msg>
        void on_message_from_transport_impl(Msg &msg) noexcept;
        void on_message_from_transport_impl(session::NewOrderSingle &msg) noexcept;
        void on_message_from_transport_impl(session::CancelReplaceRequest &msg) noexcept;
        void on_message_from_transport_impl(session::CancelRequest &msg) noexcept;

        /* Internal rejects */
        void rejecet_message_from_transport_impl(session::NewOrderSingle &msg, RejectInfo &reject_info) noexcept;
        void rejecet_message_from_transport_impl(session::CancelReplaceRequest &msg, RejectInfo &reject_info) noexcept;
        void rejecet_message_from_transport_impl(session::CancelRequest &msg, RejectInfo &reject_info) noexcept;

        template <typename Msg>
        void update_routing_info_impl(Msg &msg);

        /* Messages from the peer session to the transport */
        RejectInfo on_message_from_peer_impl(session::ExecutionReport &msg) noexcept;
        RejectInfo on_message_from_peer_impl(session::CancelReject &msg) noexcept;

    private:
        template <typename Msg>
        bool enrich_uid_from_orig_cl_ord_id(Msg &msg) noexcept;

        using ClOrdIdType = decltype(schema::NewOrderSingle::cl_ord_id);
        using ClOrdIDToUIDMap = std::unordered_map<ClOrdIdType, UID>;
        ClOrdIDToUIDMap cl_ord_id_to_uid_{optimal_order_count};
    };

    template <typename Msg>
    inline void SourceSessionProtocolA::on_message_from_transport_impl(Msg &msg) noexcept
    {
        std::cout << "unknown source message" << std::endl;
    }

    template <typename Msg>
    inline void SourceSessionProtocolA::update_routing_info_impl(Msg &msg)
    {
        if constexpr (std::is_same_v<Msg, session::NewOrderSingle>)
        {
            cl_ord_id_to_uid_.emplace(msg.msg().cl_ord_id, msg.uid());
        }
    }
}