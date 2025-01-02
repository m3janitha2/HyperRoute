#pragma once

#include <variant>

#include <application/session/ClientSessionProtocolA.h>

namespace max
{
    using ClientSessionVarient = std::variant<ClientSessionProtocolA>;
    using ClientSessionPtrVarient = std::variant<ClientSessionProtocolA*>;
}