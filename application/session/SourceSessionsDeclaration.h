#pragma once

#include <variant>

namespace max
{
    namespace protocol_a
    {
        class SourceSessionProtocolA;
    }
    
    using SourceSessionVarient = std::variant<protocol_a::SourceSessionProtocolA>;
    using SourceSessionPtrVarient = std::variant<protocol_a::SourceSessionProtocolA*>;
};