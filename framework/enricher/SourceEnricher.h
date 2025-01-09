#pragma once
#include <framework/utility/RejectInfo.h>

namespace hyper::framework
{
    struct SourceEnricher
    {
        SourceEnricher() = default;

        SourceEnricher(const SourceEnricher &) = delete;
        SourceEnricher &operator=(const SourceEnricher &) = delete;

        template <typename Msg>
        RejectInfo enrich_message_from_transport(Msg &msg) const noexcept { return RejectInfo{}; }

        template <typename Msg>
        RejectInfo enrich_message_to_transport(Msg &msg) const noexcept { return RejectInfo{}; }
    };
}