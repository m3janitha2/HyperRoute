#pragma once

#include <variant>

namespace hyper
{
    namespace protocol_a
    {
        class SourceSessionProtocolA;
    }
    
    using SourceSessionPtrVariant = std::variant<const protocol_a::SourceSessionProtocolA*>;
};