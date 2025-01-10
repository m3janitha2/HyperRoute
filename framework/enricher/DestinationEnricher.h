#pragma once

#include <framework/utility/RejectInfo.h>
#include <iostream>

namespace hyper::framework
{
    struct DestinationEnricher
    {
        DestinationEnricher() = default;

        DestinationEnricher(const DestinationEnricher &) = delete;
        DestinationEnricher &operator=(const DestinationEnricher &) = delete;

        template <typename Msg>
        RejectInfo enrich_message_to_transport([[maybe_unused]] Msg &msg) const noexcept
        {
            // std::cout << "enrich_message_to_transport:" << msg.msg() << std::endl;
            return RejectInfo{};
        }

        template <typename Msg>
        RejectInfo enrich_message_from_transport([[maybe_unused]] Msg &msg) const noexcept
        {
            // std::cout << "enrich_message_from_transport:" << msg.msg() << std::endl;
            return RejectInfo{};
        }
    };
}