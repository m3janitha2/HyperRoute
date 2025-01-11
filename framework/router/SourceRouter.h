#pragma once

#include <framework/utility/RejectInfo.h>
#include <framework/utility/Types.h>
#include <example/trading_system/session/SourceSessionsDeclaration.h>
#include <unordered_map>

namespace hyper::framework
{
    /* Route messages from the destination session to the source session.*/
    /* todox: Move this logic to the MessageStore */
    class SourceRouter
    {
    public:
        SourceRouter() = default;

        SourceRouter(const SourceRouter &) = delete;
        SourceRouter &operator=(const SourceRouter &) = delete;

        template <typename Msg>
        RejectInfo send_message_to_source(Msg &msg) noexcept;
        void update_source_routing_info(UID uid, SourceSessionPtrVarient source_session_varient) noexcept;

        std::unordered_map<UID, SourceSessionPtrVarient> uid_to_source_session_{};
    };
}