#pragma once

#include <application/session/ClientSessionsDeclaration.h>
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
        RejectInfo send_message_to_client(Msg &msg);
        void update_reverse_routing(message::UID uid, ClientSessionPtrVarient client_session_varient);

        std::unordered_map<message::UID, ClientSessionPtrVarient> uid_to_client_session_{};
    };
}