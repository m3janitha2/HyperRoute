#pragma once

#include <variant>

#include <examples/trading_system/session/SourceSessionProtocolA.h>

namespace hyper
{
    using SourceSessionVarient = std::variant<protocol_a::SourceSessionProtocolA>;
    using SourceSessionPtrVarient = std::variant<protocol_a::SourceSessionProtocolA*>;
}