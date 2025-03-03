#include <examples/trading_system/protocol/ProtocolB.h>
#include <examples/trading_system/session/DestinationSessionProtocolB.h>

namespace hyper::protocol_b
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

    ProtocolB::ProtocolB(const Configuration &config,
                         SourceRouter &source_router,
                         const ValidatorPtrVariant &validator)
        : Protocol{config, source_router, validator},
          sequence_store_(name()) {}

    void ProtocolB::on_connect_impl() noexcept
    {
    }

    void ProtocolB::on_disconnect_impl() noexcept
    {
    }

    std::size_t ProtocolB::on_data_impl(std::string_view data, Timestamp timestamp) noexcept
    {
        /* data is less than header size, don't consume */
        if (data.length() < sizeof(schema::Header))
            return 0;

        auto *const_header = reinterpret_cast<const schema::Header *>(data.data());
        auto *header = const_cast<schema::Header *>(const_header);
        /* data is less than message size, don't consume */
        if (data.length() < header->size)
            return 0;

        switch (header->msg_type)
        {
        case schema::MsgType::Logon:
        {
            protocol::Logon msg{data, timestamp};
            on_logon(msg);
            break;
        }
        case schema::MsgType::Logout:
        {
            protocol::Logout msg{data, timestamp};
            on_logout(msg);
            break;
        }
        case schema::MsgType::Heartbeat:
        {
            protocol::Heartbeat msg{data, timestamp};
            on_heartbeat(msg);
            break;
        }
        case schema::MsgType::ExecutionReport:
        {
            session::ExecutionReport msg{data, timestamp};
            impl().session().on_message_from_transport(msg);
            break;
        }
        case schema::MsgType::CancelReject:
        {
            session::CancelReject msg{data, timestamp};
            impl().session().on_message_from_transport(msg);
            break;
        }
        default:
            break;
        };

        return header->size;
    }

    void ProtocolB::on_logon(const protocol::Logon &msg) noexcept
    {
        if (auto reject_info = validate_logon(msg); reject_info != true)
        {
            send_logout();
            disconnect();
            return;
        }

        send_logon();
    }

    void ProtocolB::on_logout(const protocol::Logout &msg) noexcept
    {
        if (auto reject_info = validate_logout(msg); reject_info != true)
        {
            disconnect();
            return;
        }

        send_logout();
    }

    void ProtocolB::on_heartbeat(const protocol::Heartbeat &msg) noexcept
    {
        if (auto reject_info = validate_heartbeat(msg); reject_info != true)
        {
            disconnect();
            return;
        }
    }

    void ProtocolB::send_logon() noexcept
    {
        schema::Logon logon{};
        protocol::Logon msg{logon};
        if (auto reject_info = send_to_transport(msg);
            reject_info != true)
            disconnect();
    }

    void ProtocolB::send_logout() noexcept
    {
        schema::Logout logout{};
        protocol::Logout msg{logout};
        if (auto reject_info = send_to_transport(msg);
            reject_info != true)
            disconnect();
    }

    void ProtocolB::send_heartbeat() noexcept
    {
        schema::Heartbeat heartbeat{};
        protocol::Heartbeat msg{heartbeat};
        if (auto reject_info = send_to_transport(msg);
            reject_info != true)
            disconnect();
    }

    SessionRejectInfo ProtocolB::validate_logon([[maybe_unused]] const protocol::Logon &msg) noexcept
    {
        return SessionRejectInfo{};
    }

    SessionRejectInfo ProtocolB::validate_logout([[maybe_unused]] const protocol::Logout &msg) noexcept
    {
        return SessionRejectInfo{};
    }

    SessionRejectInfo ProtocolB::validate_heartbeat([[maybe_unused]] const protocol::Heartbeat &msg) noexcept
    {
        return SessionRejectInfo{};
    }

    void ProtocolB::disconnect() noexcept
    {
        if (auto reject_info = impl().transport().disconnect(); reject_info != true)
        {
            std::cerr << "Failed to discconnect: " << reject_info << std::endl;
        }
    }
}