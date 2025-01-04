#include <application/session/ProtocolB.h>

namespace max::protocol_b
{
    const char *to_chars(SessionRejectCode code)
    {
        using enum SessionRejectCode;
        switch (code)
        {
        case Invalid_Logon:
            return "Invalid_Logon";
        case Invalid_heartbeat:
            return "Invalid_heartbeat";
        default:
            return "Unknown";
        }
    }

    std::string to_string(SessionRejectCode code)
    {
        return to_chars(code);
    }

    ProtocolB::ProtocolB(ReverseRouter &reverse_router)
        : session_(reverse_router, transport_)
    {
    }

    void ProtocolB::on_connect()
    {
    }

    void ProtocolB::on_disconnect()
    {
    }

    std::size_t ProtocolB::on_data(std::string_view data)
    {
        auto *const_header = reinterpret_cast<const schema::Header *>(data.data());
        auto *header = const_cast<schema::Header *>(const_header);
        /* data is less than header size, don't consume */
        if (data.length() < sizeof(schema::Header))
            return 0;

        /* data is less than message size, don't consume */
        if (data.length() < header->size)
            return 0;

        switch (header->msg_type)
        {
        case schema::MsgType::Logon:
        {
            auto &msg = *(reinterpret_cast<schema::Logon *>(header));
            on_logon(msg);
            break;
        }
        case schema::MsgType::Logout:
        {
            auto &msg = *(reinterpret_cast<schema::Logout *>(header));
            on_logout(msg);
            break;
        }
        case schema::MsgType::Heartbeat:
        {
            auto &msg = *(reinterpret_cast<schema::Heartbeat *>(header));
            on_heartbeat(msg);
            break;
        }
        case schema::MsgType::ExecutionReport:
        {
            auto &msg = *(reinterpret_cast<schema::ExecutionReport *>(header));
            session::ExecutionReport msg_wrap{msg};
            session_.on_message_from_transport(msg_wrap);
            break;
        }
        case schema::MsgType::CancelReject:
        {
            auto &msg = *(reinterpret_cast<schema::CancelReject *>(header));
            session::CancelReject msg_wrap{msg};
            session_.on_message_from_transport(msg_wrap);
            break;
        }
        default:
            break;
        };

        return header->size;
    }

    void ProtocolB::on_logon(schema::Logon &msg)
    {
        if (auto reject_info = validate_logon(msg); reject_info != true)
        {
            send_logout();
            transport_.disconnect();
            return;
        }

        send_logon();
    }

    void ProtocolB::on_logout(schema::Logout &msg)
    {
        if (auto reject_info = validate_logout(msg); reject_info != true)
        {
            transport_.disconnect();
            return;
        }

        send_logout();
    }

    void ProtocolB::on_heartbeat(schema::Heartbeat &msg)
    {
        if (auto reject_info = validate_heartbeat(msg); reject_info != true)
        {
            transport_.disconnect();
            return;
        }
    }

    void ProtocolB::send_logon()
    {
        schema::Logon msg{};
        send_to_transport(msg);
    }

    void ProtocolB::send_logout()
    {
        schema::Logout msg{};
        send_to_transport(msg);
    }

    void ProtocolB::send_heartbeat()
    {
        schema::Heartbeat msg{};
        send_to_transport(msg);
    }

    SessionRejectInfo ProtocolB::validate_logon(schema::Logon &msg)
    {
        return SessionRejectInfo();
    }

    SessionRejectInfo ProtocolB::validate_logout(schema::Logout &msg)
    {
        return SessionRejectInfo();
    }

    SessionRejectInfo ProtocolB::validate_heartbeat(schema::Heartbeat &msg)
    {
        return SessionRejectInfo();
    }

}
