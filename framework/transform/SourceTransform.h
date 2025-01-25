#pragma once
#include <framework/utility/RejectInfo.h>

namespace hyper::framework
{
    struct SourceTransform
    {
        SourceTransform() = default;

        SourceTransform(const SourceTransform &) = delete;
        SourceTransform &operator=(const SourceTransform &) = delete;

        template <typename Msg>
        RejectInfo transform_message_from_transport([[maybe_unused]] Msg &msg) const noexcept { return RejectInfo{}; }

        template <typename Msg>
        RejectInfo transform_message_to_transport([[maybe_unused]] Msg &msg) const noexcept { return RejectInfo{}; }
    };
}