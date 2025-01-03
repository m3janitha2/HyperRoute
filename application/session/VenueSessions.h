#pragma once

#include <variant>

#include <application/session/VenueSessionProtocolB.h>

namespace max
{
    using VenueSessionVarient = std::variant<protocol_b::VenueSessionProtocolB>;
};