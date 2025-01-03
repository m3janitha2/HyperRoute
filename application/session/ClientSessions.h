#pragma once

#include <variant>

#include <application/session/ClientSessionProtocolA.h>

namespace max
{
    using ClientSessionVarient = std::variant<protocol_a::ClientSessionProtocolA>;
    using ClientSessionPtrVarient = std::variant<protocol_a::ClientSessionProtocolA*>;
}