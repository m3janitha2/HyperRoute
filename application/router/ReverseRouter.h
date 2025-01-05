#pragma once

#include <application/session/SourceSessionsDeclaration.h>
#include <application/session/RejectInfo.h>
#include <application/message/core/Types.h>
#include <unordered_map>

namespace max
{
    // todox: move this logic to order store
    class ReverseRouter
    {
    public:
        template <typename Msg>
        RejectInfo send_message_to_source(Msg &msg);
        void update_reverse_routing(UID uid, SourceSessionPtrVarient client_session_varient);

        std::unordered_map<UID, SourceSessionPtrVarient> uid_to_source_session_{};
    };
}