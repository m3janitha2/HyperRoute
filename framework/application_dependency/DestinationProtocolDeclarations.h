#pragma once

#include <variant>
#include <memory>

namespace hyper
{
    namespace protocol_b
    {
        class ProtocolB;
    }
    using DestinationProtocolPtrVarient =
        std::variant<std::shared_ptr<protocol_b::ProtocolB>>;
};