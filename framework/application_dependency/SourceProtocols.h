#pragma once

#include <variant>
#include <memory>
#include <examples/trading_system/protocol/ProtocolA.h>

namespace hyper
{
    using SourceProtocolPtrVarient =
        std::variant<std::shared_ptr<protocol_a::ProtocolA>>;
};