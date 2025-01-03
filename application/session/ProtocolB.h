#pragma once

#include <application/session/VenueSessionProtocolB.h>
#include <application/session/RejectInfo.h>
#include <application/message/protocol_b/Messages.h>
#include <application/transport/Transport.h>
#include <type_traits>
#include <string_view>

namespace max::protocol_b
{
    /* todox: persisted sequence number */
    struct SequenceStore
    {
        std::uint64_t in_sequence_number_{0};
        std::uint64_t out_sequence_number_{0};
    };

    enum class SessionRejectCode
    {
        None,
        Invalid_Logon,
        Invalid_heartbeat,
    };

    const char *to_chars(SessionRejectCode code);
    std::string to_string(SessionRejectCode code);

    using SessionRejectInfo = RejectInfoBase<SessionRejectCode>;

    class ProtocolB
    {
    public:
        explicit ProtocolB(ReverseRouter &reverse_router);

        void on_connect();
        void on_disconnect();
        std::size_t on_data(std::string_view data);

        const VenueSessionProtocolB &session() const { return session_; }
        VenueSessionProtocolB &session() { return session_; }

        template <typename Msg>
        RejectInfo send_to_transport(Msg &msg);

        void on_logon(schema::Logon &msg);
        void on_logout(schema::Logout &msg);
        void on_heartbeat(schema::Heartbeat &msg);

        void send_logon();
        void send_logout();
        void send_heartbeat();

        SessionRejectInfo validate_logon(schema::Logon &msg);
        SessionRejectInfo validate_logout(schema::Logout &msg);
        SessionRejectInfo validate_heartbeat(schema::Heartbeat &msg);

        void persist_session_message();
        void publish_session_message();

    private:
        TransportCallbacks transport_callbacks{[this]()
                                               { on_connect(); },
                                               [this]()
                                               { on_disconnect(); },
                                               [this](std::string_view data)
                                               { return on_data(data); }};
        Transport transport_{transport_callbacks};
        SequenceStore sequence_store_{};
        VenueSessionProtocolB session_;
    };

    template <typename Msg>
    inline RejectInfo ProtocolB::send_to_transport(Msg &msg)
    {
        // reschedule heatbeat timer
        std::string_view data{reinterpret_cast<char *>(&msg), sizeof(msg)};
        return transport_.send_data(data);
    }
}