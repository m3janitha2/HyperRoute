#pragma once

#include <framework/protocol/Protocol.h>
#include <examples/trading_system/session/DestinationSessionProtocolB.h>
#include <examples/trading_system/message/protocol_b/Messages.h>

namespace hyper::protocol_b
{
    /* Protocol implementation for Protocol B */
    /* This is a primitive, FIX-like binary protocol designed to demonstrate usage */
    /* Mostly duplicated from Protocol A for implementation simplicity */

    enum class SessionRejectCode
    {
        Success,
        Invalid_Logon,
        Invalid_heartbeat,
    };

    constexpr const char *to_chars(SessionRejectCode code) noexcept;
    std::string to_string(SessionRejectCode code);

    using SessionRejectInfo = framework::ErrorInfo<SessionRejectCode>;

    class ProtocolB : public framework::Protocol<ProtocolB, DestinationSessionProtocolB>
    {
    public:
        explicit ProtocolB(const SourceRouter &source_router,
                           const ValidatorPtrVarient &validator);

        void on_connect_impl();
        void on_disconnect_impl();
        std::size_t on_data_impl(std::string_view data);

        void on_logon(schema::Logon &msg);
        void on_logout(schema::Logout &msg);
        void on_heartbeat(schema::Heartbeat &msg);

        void send_logon();
        void send_logout();
        void send_heartbeat();

        SessionRejectInfo validate_logon(schema::Logon &msg);
        SessionRejectInfo validate_logout(schema::Logout &msg);
        SessionRejectInfo validate_heartbeat(schema::Heartbeat &msg);

    private:
        /* SessionStateMachine */
    };
}