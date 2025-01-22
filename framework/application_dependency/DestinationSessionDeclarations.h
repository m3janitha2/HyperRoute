#pragma once

#include <variant>
#include <memory>

namespace hyper
{
    namespace protocol_b
    {
        class DestinationSessionProtocolB;
    }

    using DestinationSessionPtrVariant =
        std::variant<std::monostate,
                     protocol_b::DestinationSessionProtocolB *>;
};