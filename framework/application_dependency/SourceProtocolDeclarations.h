#pragma once

#include <variant>
#include <memory>

namespace hyper
{
    namespace protocol_a
    {
        class ProtocolA;
    }

    using SourceProtocolPtrVariant =
        std::variant<std::shared_ptr<protocol_a::ProtocolA>>;
};