#pragma once
#include <framework/protocol/RejectInfo.h>

namespace hyper::framework
{
    struct SourceEnricher
    {
        template <typename Msg>
        RejectInfo enrich_message_from_transport(Msg &msg) const noexcept { return RejectInfo{}; }

        template <typename Msg>
        RejectInfo enrich_message_to_transport(Msg &msg) const noexcept { return RejectInfo{}; }
    };
}