#pragma once

#include <application/session/ClientSessionsDeclaration.h>
#include <application/session/RejectInfo.h>
#include <cstdint>
#include <unordered_map>

namespace max
{
    using UID = std::uint64_t;

    // todox: move this logic to order store
    class ReverseRouter
    {
    public:
        template <typename Msg>
        RejectInfo send_message_to_client(Msg &msg);
        void update_reverse_routing(UID uid, ClientSessionPtrVarient client_session_varient);

        std::unordered_map<UID, ClientSessionPtrVarient> uid_to_client_session_{};
    };
}