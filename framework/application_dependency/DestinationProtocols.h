#pragma once

#include <variant>
#include <memory>
#include <examples/trading_system/protocol/ProtocolB.h>

namespace hyper
{
    using DestinationProtocolPtrVariant =
        std::variant<std::shared_ptr<protocol_b::ProtocolB>>;
};