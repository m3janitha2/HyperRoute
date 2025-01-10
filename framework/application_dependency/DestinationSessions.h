#pragma once

#include <variant>

#include <example/trading_system/session/DestinationSessionProtocolB.h>

namespace hyper
{
    using DestinationSessionPtrVarient = std::variant<protocol_b::DestinationSessionProtocolB *>;
};