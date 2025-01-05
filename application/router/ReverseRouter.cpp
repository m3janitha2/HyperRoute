#include <application/router/ReverseRouter.h>
#include <application/session/SourceSessions.h>
#include <application/message/protocol_a/Messages.h>

#include <variant>
#include "ReverseRouter.h"

namespace max
{
    template <typename Msg>
    RejectInfo ReverseRouter::send_message_to_source(Msg &msg)
    {
        auto uid = msg.uid();
        if (auto it = uid_to_source_session_.find(uid); it != uid_to_source_session_.end())
        {
            auto source_session_var = it->second;
            return std::visit([&msg](auto &&client_session)
                              { return std::forward<decltype(client_session)>(client_session)->on_message_from_peer(msg); },
                              source_session_var);
        }
        else
        {
            return RejectInfo("uid not found", InteranlRejectCode::To_Client_Routing_Failed);
        }
    }

    void ReverseRouter::update_reverse_routing(UID uid, SourceSessionPtrVarient client_session_varient)
    {
        uid_to_source_session_.emplace(uid, client_session_varient);
    }

    template RejectInfo ReverseRouter::send_message_to_source<protocol_a::session::ExecutionReport>(protocol_a::session::ExecutionReport &msg);
    template RejectInfo ReverseRouter::send_message_to_source<protocol_a::session::CancelReject>(protocol_a::session::CancelReject &msg);
}
