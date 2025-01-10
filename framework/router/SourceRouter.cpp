#include <framework/router/SourceRouter.h>
#include <application/session/SourceSessions.h>
#include <application/message/protocol_a/Messages.h>

#include <variant>

namespace hyper::framework
{
    template <typename Msg>
    RejectInfo SourceRouter::send_message_to_source(Msg &msg) noexcept
    {
        auto uid = msg.uid();
        if (auto it = uid_to_source_session_.find(uid); it != uid_to_source_session_.end())
        {
            auto source_session_var = it->second;
            return std::visit([&msg](auto &&source_session)
                              { return std::forward<decltype(source_session)>(source_session)->on_message_from_peer(msg); },
                              source_session_var);
        }
        else
        {
            return RejectInfo("uid not found", InteranlRejectCode::To_Source_Routing_Failed);
        }
    }

    void SourceRouter::update_routing_info(UID uid, SourceSessionPtrVarient source_session_varient) noexcept
    {
        uid_to_source_session_.emplace(uid, source_session_varient);
    }

    template RejectInfo SourceRouter::send_message_to_source<protocol_a::session::ExecutionReport>(protocol_a::session::ExecutionReport &msg);
    template RejectInfo SourceRouter::send_message_to_source<protocol_a::session::CancelReject>(protocol_a::session::CancelReject &msg);
}
