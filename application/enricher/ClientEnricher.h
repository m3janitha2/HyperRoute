#pragma once
#include <application/session/RejectInfo.h>

namespace max
{
    struct ClientEnricher
    {
        template <typename Msg>
        RejectInfo enrich_message_from_transport(Msg &msg) const noexcept { return RejectInfo{}; }

        template <typename Msg>
        RejectInfo enrich_message_to_transport(Msg &msg) const noexcept { return RejectInfo{}; }
    };
}