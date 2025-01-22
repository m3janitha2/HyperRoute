#pragma once

#include <framework/session/SourceSession.h>
#include <examples/trading_system/message/protocol_a/Messages.h>
#include <examples/trading_system/utility/utility.h>
#include <type_traits>
#include <iostream>

namespace hyper::protocol_a
{
    using RejectInfo = framework::RejectInfo;
    using SourceRouter = framework::SourceRouter;
    using Transport = framework::Transport;
    using UID = framework::UID;

    /* Application message session implementation for the source session (Protocol A) */
    
    class SourceSessionProtocolA : public framework::SourceSession<SourceSessionProtocolA>
    {
    public:
        explicit SourceSessionProtocolA(Transport &transport,
                                        const DestinationRouterPtrVariant &destination_router,
                                        SourceRouter &source_router)
            : SourceSession<SourceSessionProtocolA>{transport, destination_router, source_router} {}

        /* TransportEvents */
        void on_connect_impl() noexcept;
        void on_disconnect_impl() noexcept;

        /* Messages from transport */
        RejectInfo handle_message_from_transport_impl(session::NewOrderSingle &msg) noexcept;
        RejectInfo handle_message_from_transport_impl(session::CancelReplaceRequest &msg) noexcept;
        RejectInfo handle_message_from_transport_impl(session::CancelRequest &msg) noexcept;

        /* Internal rejects */
        void reject_message_from_transport_impl(session::NewOrderSingle &msg,
                                                RejectInfo &reject_info) noexcept;
        void reject_message_from_transport_impl(session::CancelReplaceRequest &msg,
                                                RejectInfo &reject_info) noexcept;
        void reject_message_from_transport_impl(session::CancelRequest &msg,
                                                RejectInfo &reject_info) noexcept;

        template <typename Msg>
        void update_routing_info_impl(Msg &msg) noexcept;

        /* Messages from the peer session to the transport */
        RejectInfo on_message_from_peer_impl(session::ExecutionReport &msg) noexcept;
        RejectInfo on_message_from_peer_impl(session::CancelReject &msg) noexcept;

    private:
        RejectInfo enrich_uid_from_cl_ord_id(session::NewOrderSingle &msg) noexcept;
        template <typename Msg>
        RejectInfo enrich_uid_from_orig_cl_ord_id(Msg &msg) noexcept;

        using ClOrdIdType = decltype(schema::NewOrderSingle::cl_ord_id);
        using ClOrdIDToUIDMap = std::unordered_map<ClOrdIdType, UID>;
        ClOrdIDToUIDMap cl_ord_id_to_uid_{optimal_order_count};
    };

    template <typename Msg>
    inline void SourceSessionProtocolA::update_routing_info_impl(Msg &msg) noexcept
    try
    {
        if constexpr (std::is_same_v<Msg, session::NewOrderSingle>)
        {
            if (auto [it, ret] = cl_ord_id_to_uid_.try_emplace(msg.msg().cl_ord_id, msg.uid());
                ret != true)
                std::cout << "Failed to update source routing info. cl_ord_id:" << msg.msg().cl_ord_id
                          << " uid:" << msg.uid() << std::endl;
        }
    }
    catch (std::exception &err)
    {
        std::cout << "Critical Error. Failed to update source routing info. cl_ord_id:" << msg.msg().cl_ord_id
                  << " uid:" << msg.uid() << " error:" << err.what() << std::endl;
    }
}