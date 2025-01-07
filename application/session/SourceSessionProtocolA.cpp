#include <application/session/SourceSessionProtocolA.h>
#include <framework/message/UIDGenerator.h>

namespace hyper::protocol_a
{
    using UIDGenerator = framework::UIDGenerator;
    using InteranlRejectCode = framework::InteranlRejectCode;

    void SourceSessionProtocolA::on_connect_impl() {}

    void SourceSessionProtocolA::on_disconnect_impl() {}

    void SourceSessionProtocolA::on_message_from_transport_impl(session::NewOrderSingle &msg)
    {
        auto uid = UIDGenerator::instance().get_next_uid();
        msg.uid(uid);

        procoess_message_from_transport(msg);
    }

    void SourceSessionProtocolA::on_message_from_transport_impl(session::CancelReplaceRequest &msg)
    {
        if (auto it = cl_ord_id_to_uid_.find(msg.msg().orig_cl_ord_id); it != cl_ord_id_to_uid_.end())
        {
            auto uid = it->second;
            msg.uid(uid);
        }
        else
        {
            RejectInfo reject_info{"Invalid ClOrdID", InteranlRejectCode::Invalid_Orig_Cl_Ord_ID};
            rejecet_message_from_transport_impl(msg, reject_info);
            return;
        }

        procoess_message_from_transport(msg);
    }

    void SourceSessionProtocolA::on_message_from_transport_impl(session::CancelRequest &msg)
    {
        if (auto it = cl_ord_id_to_uid_.find(msg.msg().orig_cl_ord_id); it != cl_ord_id_to_uid_.end())
        {
            auto uid = it->second;
            msg.uid(uid);
        }
        else
        {
            RejectInfo reject_info{"Invalid ClOrdID", InteranlRejectCode::Invalid_Orig_Cl_Ord_ID};
            rejecet_message_from_transport_impl(msg, reject_info);
            return;
        }

        procoess_message_from_transport(msg);
    }

    void SourceSessionProtocolA::rejecet_message_from_transport_impl(session::NewOrderSingle &msg, RejectInfo &reject_info)
    {
        std::cout << "msg rejected: " << msg << " reason: " << reject_info << std::endl;
    }

    void SourceSessionProtocolA::rejecet_message_from_transport_impl(session::CancelReplaceRequest &msg, RejectInfo &reject_info)
    {
        std::cout << "msg rejected: " << msg << " reason: " << reject_info << std::endl;
    }

    void SourceSessionProtocolA::rejecet_message_from_transport_impl(session::CancelRequest &msg, RejectInfo &reject_info)
    {
        std::cout << "msg rejected: " << msg << " reason: " << reject_info << std::endl;
    }

    RejectInfo SourceSessionProtocolA::on_message_from_peer_impl(session::ExecutionReport &msg)
    {
        return procoess_message_to_transport(msg);
    }

    RejectInfo SourceSessionProtocolA::on_message_from_peer_impl(session::CancelReject &msg)
    {
        return procoess_message_to_transport(msg);
    }
}