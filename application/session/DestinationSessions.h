#pragma once

#include <variant>

#include <application/session/DestinationSessionProtocolB.h>

namespace max
{
    //using DestinationSessionPtrVarient = std::variant<protocol_b::DestinationSessionProtocolB>;
    using DestinationSessionPtrVarient = std::variant<protocol_b::DestinationSessionProtocolB*>;
};