#pragma once

#include <variant>

namespace max
{
    class ClientSessionProtocolA;
    using ClientSessionVarient = std::variant<ClientSessionProtocolA>;

    using ClientSessionPtrVarient = std::variant<ClientSessionProtocolA*>;
};