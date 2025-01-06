#include <application/session/ProtocolA.h>
#include <application/session/SourceSessionProtocolA.h>

namespace max::protocol_a
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

    ProtocolA::ProtocolA(DestinationRouterPtrVarient &destination_router,
                         SourceRouter &source_router)
        : session_(destination_router, source_router, transport_)
    {
    }

    void ProtocolA::on_connect()
    {
    }

    void ProtocolA::on_disconnect()
    {
    }

    std::size_t ProtocolA::on_data(std::string_view data)
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
        case schema::MsgType::NewOrderSingle:
        {
            session::NewOrderSingle msg{data};
            session_.on_message_from_transport(msg);
            break;
        }
        case schema::MsgType::CancelReplaceRequest:
        {
            session::CancelReplaceRequest msg{data};
            session_.on_message_from_transport(msg);
            break;
        }
        case schema::MsgType::CancelRequest:
        {
            session::CancelRequest msg{data};
            session_.on_message_from_transport(msg);
            break;
        }
        default:
            break;
        };

        return header->size;
    }

    void ProtocolA::on_logon(schema::Logon &msg)
    {
        if (auto reject_info = validate_logon(msg); reject_info != true)
        {
            send_logout();
            transport_.disconnect();
            return;
        }

        send_logon();
    }

    void ProtocolA::on_logout(schema::Logout &msg)
    {
        if (auto reject_info = validate_logout(msg); reject_info != true)
        {
            transport_.disconnect();
            return;
        }

        send_logout();
    }

    void ProtocolA::on_heartbeat(schema::Heartbeat &msg)
    {
        if (auto reject_info = validate_heartbeat(msg); reject_info != true)
        {
            transport_.disconnect();
            return;
        }
    }

    void ProtocolA::send_logon()
    {
        schema::Logon msg{};
        auto reject_info = send_to_transport(msg);
    }

    void ProtocolA::send_logout()
    {
        schema::Logout msg{};
        auto reject_info = send_to_transport(msg);
    }

    void ProtocolA::send_heartbeat()
    {
        schema::Heartbeat msg{};
        auto reject_info = send_to_transport(msg);
    }

    SessionRejectInfo protocol_a::ProtocolA::validate_logon(schema::Logon &msg)
    {
        return SessionRejectInfo();
    }

    SessionRejectInfo protocol_a::ProtocolA::validate_logout(schema::Logout &msg)
    {
        return SessionRejectInfo();
    }

    SessionRejectInfo protocol_a::ProtocolA::validate_heartbeat(schema::Heartbeat &msg)
    {
        return SessionRejectInfo();
    }
}
