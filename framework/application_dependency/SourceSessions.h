#pragma once

#include <variant>

#include <examples/trading_system/session/SourceSessionProtocolA.h>

namespace hyper
{
    using SourceSessionPtrVariant = std::variant<protocol_a::SourceSessionProtocolA*>;
}