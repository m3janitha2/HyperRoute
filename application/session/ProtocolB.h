#pragma once

#include <framework/protocol/Protocol.h>
#include <application/session/DestinationSessionProtocolB.h>
#include <application/message/protocol_b/Messages.h>

namespace hyper::protocol_b
{
    enum class SessionRejectCode
    {
        Success,
        Invalid_Logon,
        Invalid_heartbeat,
    };

    constexpr const char *to_chars(SessionRejectCode code) noexcept;
    std::string to_string(SessionRejectCode code);

    using SessionRejectInfo = framework::ErrorInfo<SessionRejectCode>;

    class ProtocolB : public framework::Protocol<ProtocolB>
    {
    public:
        explicit ProtocolB(SourceRouter &source_router);

        void on_connect_impl();
        void on_disconnect_impl();
        std::size_t on_data_impl(std::string_view data);

        const DestinationSessionProtocolB &session() const { return session_; }
        DestinationSessionProtocolB &session() { return session_; }

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
        DestinationSessionProtocolB session_;
    };
}