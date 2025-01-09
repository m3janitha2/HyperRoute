#pragma once

#include <variant>

#include <application/session/DestinationSessionProtocolB.h>

namespace hyper
{
    using DestinationSessionPtrVarient = std::variant<protocol_b::DestinationSessionProtocolB*>;
};