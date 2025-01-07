#pragma once

#include <framework/protocol/RejectInfo.h>
#include <iostream>

namespace hyper::framework
{
    struct DestinationEnricher
    {
        template <typename Msg>
        RejectInfo enrich_message_to_transport(Msg &msg) const noexcept
        {
            //std::cout << "enrich_message_to_transport:" << msg.msg() << std::endl;
            return RejectInfo{};
        }

        template <typename Msg>
        RejectInfo enrich_message_from_transport(Msg &msg) const noexcept
        {
            //std::cout << "enrich_message_from_transport:" << msg.msg() << std::endl;
            return RejectInfo{};
        }
    };
}