#pragma once

#include <framework/protocol/RejectInfo.h>
#include <framework/utility/Types.h>
#include <application/session/SourceSessionsDeclaration.h>
#include <unordered_map>

namespace hyper::framework
{
    // todox: move this logic to message store
    class SourceRouter
    {
    public:
        template <typename Msg>
        RejectInfo send_message_to_source(Msg &msg);
        void update_reverse_routing(UID uid, SourceSessionPtrVarient source_session_varient);

        std::unordered_map<UID, SourceSessionPtrVarient> uid_to_source_session_{};
    };
}