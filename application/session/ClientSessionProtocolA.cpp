#include <application/session/ClientSessionProtocolA.h>

namespace max::protocol_a
{
    void ClientSessionProtocolA::on_connect_impl() {}

    void ClientSessionProtocolA::on_disconnect_impl() {}

    void ClientSessionProtocolA::on_message_impl(session::NewOrderSingle &msg)
    {
        procoess_message_from_client(msg);
    }

    void ClientSessionProtocolA::on_message_impl(session::CancelReplaceRequest &msg)
    {
        procoess_message_from_client(msg);
    }

    void ClientSessionProtocolA::on_message_impl(session::CancelRequest &msg)
    {
        procoess_message_from_client(msg);
    }

    void ClientSessionProtocolA::reject_client_message_impl(session::NewOrderSingle &msg, RejectInfo &reject_info)
    {
    }

    void ClientSessionProtocolA::reject_client_message_impl(session::CancelReplaceRequest &msg, RejectInfo &reject_info)
    {
    }

    void ClientSessionProtocolA::reject_client_message_impl(session::CancelRequest &msg, RejectInfo &reject_info)
    {
    }

    RejectInfo ClientSessionProtocolA::on_venue_message_impl(session::ExecutionReport &msg)
    {
        return procoess_message_to_client(msg);
    }

    RejectInfo ClientSessionProtocolA::on_venue_message_impl(session::CancelReject &msg)
    {
        return procoess_message_to_client(msg);
    }
}