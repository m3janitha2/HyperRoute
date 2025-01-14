#pragma once

#include <variant>
#include <examples/trading_system/session/DestinationSessionProtocolB.h>

namespace hyper
{
    namespace protocol_b
    {
        class DestinationSessionProtocolB;
    }

    using DestinationSessionPtrVarient = std::variant<std::monostate,
                                                      protocol_b::DestinationSessionProtocolB *>;
};