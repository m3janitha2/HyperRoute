#pragma once

#include <application/session/ClientSessions.h>

namespace max
{
    struct ClientSessionInfo
    {
        explicit ClientSessionInfo(const ClientSessionVarient &client_session)
            : client_session_(client_session) {}

        const ClientSessionVarient client_session_;
    };
};