#pragma once

#include <framework/utility/RejectInfo.h>
#include <iostream>

namespace hyper::framework
{
    struct DestinationTransform
    {
        DestinationTransform() = default;

        DestinationTransform(const DestinationTransform &) = delete;
        DestinationTransform &operator=(const DestinationTransform &) = delete;

        template <typename Msg>
        RejectInfo transform_message_to_transport([[maybe_unused]] Msg &msg) const noexcept
        {
            // std::cout << "transform_message_to_transport:" << msg.msg() << std::endl;
            return RejectInfo{};
        }

        template <typename Msg>
        RejectInfo transform_message_from_transport([[maybe_unused]] Msg &msg) const noexcept
        {
            // std::cout << "transform_message_from_transport:" << msg.msg() << std::endl;
            return RejectInfo{};
        }
    };
}