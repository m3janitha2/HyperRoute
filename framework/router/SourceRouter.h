#pragma once

#include <framework/utility/RejectInfo.h>
#include <framework/utility/Types.h>
#include <framework/application_dependency/SourceSessionDeclarations.h>
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
        RejectInfo send_message_to_source(Msg &msg) const noexcept;
        void update_source_routing_info(UID uid, SourceSessionPtrVariant source_session_Variant) const noexcept;

    private:
        /* tbb::concurrent_hash_map for concurrent multi-threaded access */
        mutable std::unordered_map<UID, SourceSessionPtrVariant> uid_to_source_session_{};
    };
}