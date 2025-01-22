#pragma once

#include <variant>
#include <memory>

namespace hyper
{
    namespace protocol_b
    {
        class ProtocolB;
    }
    using DestinationProtocolPtrVariant =
        std::variant<std::shared_ptr<protocol_b::ProtocolB>>;
};