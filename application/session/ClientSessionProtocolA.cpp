#include <application/session/ClientSessionProtocolA.h>

namespace max::protocol_a
{
    void ClientSessionProtocolA::on_connect_impl() {}

    void ClientSessionProtocolA::on_disconnect_impl() {}

    void ClientSessionProtocolA::on_message_from_transport_impl(session::NewOrderSingle &msg)
    {
        procoess_message_from_transport(msg);
    }

    void ClientSessionProtocolA::on_message_from_transport_impl(session::CancelReplaceRequest &msg)
    {
        procoess_message_from_transport(msg);
    }

    void ClientSessionProtocolA::on_message_from_transport_impl(session::CancelRequest &msg)
    {
        procoess_message_from_transport(msg);
    }

    void ClientSessionProtocolA::rejecet_message_from_transport_impl(session::NewOrderSingle &msg, RejectInfo &reject_info)
    {
    }

    void ClientSessionProtocolA::rejecet_message_from_transport_impl(session::CancelReplaceRequest &msg, RejectInfo &reject_info)
    {
    }

    void ClientSessionProtocolA::rejecet_message_from_transport_impl(session::CancelRequest &msg, RejectInfo &reject_info)
    {
    }

    RejectInfo ClientSessionProtocolA::on_message_from_peer_impl(session::ExecutionReport &msg)
    {
        return procoess_message_to_transport(msg);
    }

    RejectInfo ClientSessionProtocolA::on_message_from_peer_impl(session::CancelReject &msg)
    {
        return procoess_message_to_transport(msg);
    }
}