#pragma once

#include <variant>
#include <memory>

namespace hyper
{
    namespace protocol_b
    {
        class DestinationSessionProtocolB;
    }

    using DestinationSessionPtrVarient =
        std::variant<std::monostate,
                     protocol_b::DestinationSessionProtocolB *>;
};