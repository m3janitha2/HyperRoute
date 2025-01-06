#pragma once
#include <framework/protocol/RejectInfo.h>

namespace max::framework
{
    struct DestinationEnricher
    {
        template <typename Msg>
        RejectInfo enrich_message_to_transport(Msg &msg) const noexcept { return RejectInfo{}; }

        template <typename Msg>
        RejectInfo enrich_message_from_transport(Msg &msg) const noexcept { return RejectInfo{}; }
    };
}