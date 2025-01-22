#pragma once

#include <variant>

namespace hyper
{
    namespace protocol_a
    {
        class SourceSessionProtocolA;
    }
    
    using SourceSessionPtrVariant = std::variant<protocol_a::SourceSessionProtocolA*>;
};