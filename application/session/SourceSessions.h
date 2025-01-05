#pragma once

#include <variant>

#include <application/session/SourceSessionProtocolA.h>

namespace max
{
    using SourceSessionVarient = std::variant<protocol_a::SourceSessionProtocolA>;
    using SourceSessionPtrVarient = std::variant<protocol_a::SourceSessionProtocolA*>;
}