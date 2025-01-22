#pragma once

#include <variant>
#include <memory>
#include <examples/trading_system/session/DestinationSessionProtocolB.h>

namespace hyper
{
    using DestinationSessionPtrVariant =
        std::variant<protocol_b::DestinationSessionProtocolB *>;
};