#include <examples/trading_system/session/SourceSessionProtocolA.h>
#include <examples/trading_system/protocol/ProtocolA.h>
#include <framework/message/UIDGenerator.h>

namespace hyper::protocol_a
{
    using UIDGenerator = framework::UIDGenerator;
    using InteranlRejectCode = framework::InteranlRejectCode;

    SourceSessionProtocolA::SourceSessionProtocolA(ProtocolA &protocol,
                                                   const DestinationRouterPtrVariant &destination_router,
                                                   SourceRouter &source_router)
        : SourceSession{protocol, destination_router, source_router} {}

    void SourceSessionProtocolA::on_connect_impl() noexcept {}

    void SourceSessionProtocolA::on_disconnect_impl() noexcept {}

    RejectInfo SourceSessionProtocolA::handle_message_from_transport_impl(session::NewOrderSingle &msg) const noexcept
    {
        return transform_uid_from_cl_ord_id(msg);
    }

    RejectInfo SourceSessionProtocolA::handle_message_from_transport_impl(session::CancelReplaceRequest &msg) const noexcept
    {
        return transform_uid_from_orig_cl_ord_id(msg);
    }

    RejectInfo SourceSessionProtocolA::handle_message_from_transport_impl(session::CancelRequest &msg) const noexcept
    {
        return transform_uid_from_orig_cl_ord_id(msg);
    }

    void SourceSessionProtocolA::reject_message_from_transport_impl(session::NewOrderSingle &msg,
                                                                    RejectInfo &reject_info) const noexcept
    {
        std::cout << "msg rejected: " << msg << " reason: " << reject_info << std::endl;
        schema::ExecutionReport reject;
        session::ExecutionReport dst_msg(reject);
        /* create reject */
        send_message_to_transport_ignoring_errors(dst_msg);
    }

    void SourceSessionProtocolA::reject_message_from_transport_impl(session::CancelReplaceRequest &msg,
                                                                    RejectInfo &reject_info) const noexcept
    {
        std::cout << "msg rejected: " << msg << " reason: " << reject_info << std::endl;
        schema::ExecutionReport reject;
        session::ExecutionReport dst_msg(reject);
        /* create reject */
        send_message_to_transport_ignoring_errors(dst_msg);
    }

    void SourceSessionProtocolA::reject_message_from_transport_impl(session::CancelRequest &msg,
                                                                    RejectInfo &reject_info) const noexcept
    {
        std::cout << "msg rejected: " << msg << " reason: " << reject_info << std::endl;
        schema::CancelReject reject;
        session::CancelReject dst_msg(reject);
        /* create reject */
        send_message_to_transport_ignoring_errors(dst_msg);
    }

    RejectInfo SourceSessionProtocolA::on_message_from_peer_impl(session::ExecutionReport &msg) const noexcept
    {
        return procoess_message_to_transport(msg);
    }

    RejectInfo SourceSessionProtocolA::on_message_from_peer_impl(session::CancelReject &msg) const noexcept
    {
        return procoess_message_to_transport(msg);
    }

    RejectInfo SourceSessionProtocolA::transform_uid_from_cl_ord_id(session::NewOrderSingle &msg) const noexcept
    {
        auto uid = UIDGenerator::instance().get_next_uid();
        msg.uid(uid);
        return RejectInfo();
    }

    template <typename Msg>
    RejectInfo SourceSessionProtocolA::transform_uid_from_orig_cl_ord_id(Msg &msg) const noexcept
    {
        if (auto it = cl_ord_id_to_uid_.find(msg.orig_cl_ord_id());
            it == cl_ord_id_to_uid_.end()) [[unlikely]]
            return RejectInfo{"Invalid Origianl ClOrdID", InteranlRejectCode::SourceSession_Invalid_Original_Msg_ID};
        else
        {
            auto uid = it->second;
            msg.uid(uid);
        }
        return RejectInfo{};
    }
}