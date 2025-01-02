#include <application/session/ClientSessionProtocolA.h>

namespace max
{
    void ClientSessionProtocolA::on_data_impl()
    {
        protocol_a::NewOrderSingle msg{2, 3};
        on_client_message_impl(msg);
    }

    void ClientSessionProtocolA::on_connect_impl() {}

    void ClientSessionProtocolA::on_disconnect_impl() {}

    void ClientSessionProtocolA::on_client_message_impl(protocol_a::NewOrderSingle &msg)
    {
        procoess_message_from_client(msg);
    }

    void ClientSessionProtocolA::on_client_message_impl(protocol_a::CancelReplaceRequest &msg)
    {
        procoess_message_from_client(msg);
    }

    void ClientSessionProtocolA::on_client_message_impl(protocol_a::CancelRequest &msg)
    {
        procoess_message_from_client(msg);
    }

    void ClientSessionProtocolA::reject_client_message_impl(protocol_a::NewOrderSingle &msg, RejectInfo &reject_info)
    {
    }

    void ClientSessionProtocolA::reject_client_message_impl(protocol_a::CancelReplaceRequest &msg, RejectInfo &reject_info)
    {
    }

    void ClientSessionProtocolA::reject_client_message_impl(protocol_a::CancelRequest &msg, RejectInfo &reject_info)
    {
    }

    RejectInfo ClientSessionProtocolA::on_venue_message_impl(protocol_a::ExecutionReport &msg)
    {
        return procoess_message_to_client(msg);
    }

    RejectInfo ClientSessionProtocolA::on_venue_message_impl(protocol_a::CancelReject &msg)
    {
        return procoess_message_to_client(msg);
    }
}