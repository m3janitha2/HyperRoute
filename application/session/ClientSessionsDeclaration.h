#pragma once

#include <variant>

namespace max
{
    namespace protocol_a
    {
        class ClientSessionProtocolA;
    }
    
    using ClientSessionVarient = std::variant<protocol_a::ClientSessionProtocolA>;
    using ClientSessionPtrVarient = std::variant<protocol_a::ClientSessionProtocolA*>;
};