#include <examples/trading_system/protocol/ProtocolA.h>

namespace hyper::protocol_a
{
    constexpr const char *to_chars(SessionRejectCode code) noexcept
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

    ProtocolA::ProtocolA(const Configuration &config,
                         const DestinationRouterPtrVariant &destination_router,
                         SourceRouter &source_router)
        : Protocol{config, destination_router, source_router} {}

    void ProtocolA::on_connect_impl()
    {
    }

    void ProtocolA::on_disconnect_impl()
    {
    }

    std::size_t ProtocolA::on_data_impl(std::string_view data, Timestamp timestamp)
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
            session::NewOrderSingle msg{data, timestamp};
            impl().session().on_message_from_transport(msg);
            break;
        }
        case schema::MsgType::CancelReplaceRequest:
        {
            session::CancelReplaceRequest msg{data, timestamp};
            impl().session().on_message_from_transport(msg);
            break;
        }
        case schema::MsgType::CancelRequest:
        {
            session::CancelRequest msg{data, timestamp};
            impl().session().on_message_from_transport(msg);
            break;
        }
        default:
            std::cerr << "invalid message. data[" << data << "]" << std::endl;
            break;
        };

        return header->size;
    }

    void ProtocolA::on_logon(schema::Logon &msg)
    {
        if (auto reject_info = validate_logon(msg); reject_info != true)
        {
            send_logout();
            disconnect();
            return;
        }

        send_logon();
    }

    void ProtocolA::on_logout(schema::Logout &msg)
    {
        if (auto reject_info = validate_logout(msg); reject_info != true)
        {
            disconnect();
            return;
        }

        send_logout();
    }

    void ProtocolA::on_heartbeat(schema::Heartbeat &msg)
    {
        if (auto reject_info = validate_heartbeat(msg); reject_info != true)
        {
            disconnect();
            return;
        }
    }

    void ProtocolA::send_logon()
    {
        schema::Logon msg{};
        if (auto reject_info = send_to_transport(msg);
            reject_info != true)
            disconnect();
    }

    void ProtocolA::send_logout()
    {
        schema::Logout msg{};
        if (auto reject_info = send_to_transport(msg);
            reject_info != true)
            disconnect();
    }

    void ProtocolA::send_heartbeat()
    {
        schema::Heartbeat msg{};
        if (auto reject_info = send_to_transport(msg);
            reject_info != true)
            disconnect();
    }

    SessionRejectInfo ProtocolA::validate_logon([[maybe_unused]] schema::Logon &msg)
    {
        return SessionRejectInfo{};
    }

    SessionRejectInfo ProtocolA::validate_logout([[maybe_unused]] schema::Logout &msg)
    {
        return SessionRejectInfo{};
    }

    SessionRejectInfo ProtocolA::validate_heartbeat([[maybe_unused]] schema::Heartbeat &msg)
    {
        return SessionRejectInfo{};
    }

    void ProtocolA::disconnect() noexcept
    {
        if (auto reject_info = impl().transport().disconnect(); reject_info != true)
        {
            std::cerr << "Failed to discconnect: " << reject_info << std::endl;
        }
    }
}
