#pragma once

#include <framework/protocol/Protocol.h>
#include <examples/trading_system/session/SourceSessionProtocolA.h>
#include <examples/trading_system/message/protocol_a/Messages.h>
#include <type_traits>

namespace hyper::protocol_a
{
    /* Protocol implementation for Protocol A */
    /* This is a primitive, FIX-like binary protocol designed to demonstrate usage */

    enum class SessionRejectCode
    {
        Success,
        Invalid_Logon,
        Invalid_heartbeat,
    };

    constexpr const char *to_chars(SessionRejectCode code) noexcept;
    std::string to_string(SessionRejectCode code);

    using SessionRejectInfo = framework::ErrorInfo<SessionRejectCode>;
    using Configuration = framework::Configuration;

    class ProtocolA : public framework::Protocol<ProtocolA, SourceSessionProtocolA>
    {
    public:
        explicit ProtocolA(const Configuration &config,
                           const DestinationRouterPtrVarient &destination_router,
                           const SourceRouter &source_router);

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