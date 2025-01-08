#pragma once

#include <framework/protocol/Protocol.h>
#include <application/session/SourceSessionProtocolA.h>
#include <application/message/protocol_a/Messages.h>
#include <type_traits>

namespace hyper::protocol_a
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

    class ProtocolA : public framework::Protocol<ProtocolA>
    {
    public:
        explicit ProtocolA(DestinationRouterPtrVarient &destination_router,
                           SourceRouter &source_router);

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
        SourceSessionProtocolA session_;
    };
}