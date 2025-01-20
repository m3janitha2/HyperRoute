#pragma once

#include <variant>
#include <memory>

namespace hyper
{
    namespace protocol_a
    {
        class ProtocolA;
    }

    using SourceProtocolPtrVarient =
        std::variant<std::shared_ptr<protocol_a::ProtocolA>>;
};