#pragma once

#include <variant>

#include <application/session/DestinationSessionProtocolB.h>

namespace max
{
    namespace protocol_b
    {
        class DestinationSessionProtocolB;
    }

    using DestinationSessionVarient = std::variant<protocol_b::DestinationSessionProtocolB>;
    using DestinationSessionPtrVarient = std::variant<std::monostate, protocol_b::DestinationSessionProtocolB*>;
};