#pragma once

#include <variant>

#include <application/session/DestinationSessionProtocolB.h>

namespace max
{
    using DestinationSessionVarient = std::variant<protocol_b::DestinationSessionProtocolB>;
    using DestinationSessionPtrVarient = std::variant<std::monostate, protocol_b::DestinationSessionProtocolB*>;
};