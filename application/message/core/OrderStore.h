#pragma once

#include <application/message/core/Types.h>
#include <application/session/RejectInfo.h>
#include <application/session/DestinationSessionsDeclarations.h>
#include <unordered_map>

namespace max
{
    class OrderStore
    {
    public:
        std::unordered_map<UID, DestinationSessionPtrVarient> uid_to_destination_session_{};

        RejectInfo add_destination_session_mapping(UID uid, DestinationSessionPtrVarient destination_session_ptr);
    };
}