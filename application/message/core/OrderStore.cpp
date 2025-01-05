#include <application/message/core/OrderStore.h>
#include <application/session/DestinationSessions.h>

namespace max
{
    RejectInfo OrderStore::add_destination_session_mapping(UID uid, DestinationSessionPtrVarient destination_session_ptr)
    {
        if (auto [it, ret] = uid_to_destination_session_.emplace(uid, destination_session_ptr); ret != true)
        {
            return RejectInfo{"Duplicate UID in venue session mapping", InteranlRejectCode::Duplicate_UID_In_Destination_Session_Mapping};
        }

        return RejectInfo{};
    }
}