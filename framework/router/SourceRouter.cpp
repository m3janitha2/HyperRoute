#include <framework/router/SourceRouter.h>
#include <framework/application_dependency/SourceSessions.h>
#include <examples/trading_system/message/protocol_a/Messages.h>

#include <variant>

namespace hyper::framework
{
    template <typename Msg>
    RejectInfo SourceRouter::send_message_to_source(Msg &msg) const noexcept
    {
        auto uid = msg.uid();
        if (auto it = uid_to_source_session_.find(uid); it != uid_to_source_session_.end())
        {
            auto source_session_var = it->second;
            return std::visit([&msg]<typename Source>(Source &&source_session)
                              { return std::forward<Source>(source_session)->on_message_from_peer(msg); },
                              source_session_var);
        }
        else
        {
            return RejectInfo("uid not found", InteranlRejectCode::SourceRouter_Routing_Failed);
        }
    }

    void SourceRouter::update_source_routing_info(UID uid, SourceSessionPtrVariant source_session_Variant) const noexcept
    try
    {
        if (auto [it, ret] = uid_to_source_session_.try_emplace(uid, source_session_Variant);
            ret != true)
            std::cout << "Failed update source routing for uid: " << uid << std::endl;
    }
    catch (std::exception &err)
    {
        std::cout << "Critical Error. Failed update source routing for uid: " << uid
                  << " error:" << err.what() << std::endl;
    }

    /* todox: this is odd */
    template RejectInfo SourceRouter::send_message_to_source<protocol_a::session::ExecutionReport>(protocol_a::session::ExecutionReport &msg) const noexcept;
    template RejectInfo SourceRouter::send_message_to_source<protocol_a::session::CancelReject>(protocol_a::session::CancelReject &msg) const noexcept;
}
