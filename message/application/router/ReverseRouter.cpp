#include <application/router/ReverseRouter.h>
#include <application/session/ClientSessions.h>

#include <variant>
#include "ReverseRouter.h"

namespace max
{
    template <typename Msg>
    RejectInfo ReverseRouter::send_message_to_client(Msg &msg)
    {
        UID uid{123};
        if (auto it = uid_to_client_session_.find(uid); it != uid_to_client_session_.end())
        {
            auto client_session_var = it->second;
            return std::visit([&msg](auto &&client_session)
                              { return std::forward<decltype(client_session)>(client_session)->on_venue_message(msg); },
                              client_session_var);
        }
        else
        {
            return RejectInfo("uid not found", InteranlRejectCode::To_Client_Routing_Failed);
        }
    }

    void ReverseRouter::update_reverse_routing(UID uid, ClientSessionPtrVarient client_session_varient)
    {
        uid_to_client_session_.emplace(uid, client_session_varient);
    }

    template RejectInfo ReverseRouter::send_message_to_client<protocol_a::ExecutionReport>(protocol_a::ExecutionReport &msg);
    template RejectInfo ReverseRouter::send_message_to_client<protocol_a::CancelReject>(protocol_a::CancelReject &msg);
}
