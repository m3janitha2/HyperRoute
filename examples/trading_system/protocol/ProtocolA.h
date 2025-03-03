#pragma once

#include <framework/protocol/Protocol.h>
#include <framework/application_dependency/DestinationRouterDeclarations.h>
#include <framework/application_dependency/SourceRouterDeclarations.h>
#include <framework/sequence_store/SequenceStore.h>
#include <examples/trading_system/message/protocol_a/Messages.h>
#include <type_traits>

namespace hyper::protocol_a
{
    /* Protocol implementation for Protocol A */
    /* This is a primitive, FIX-like binary protocol designed to demonstrate usage */

    class SourceSessionProtocolA;

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
    using Timestamp = framework::Timestamp;
    using SourceRouter = framework::SourceRouter;
    using SequenceStore = framework::SequenceStore<std::uint64_t>;

    class ProtocolA : public framework::Protocol<ProtocolA, SourceSessionProtocolA>
    {
    public:
        explicit ProtocolA(const Configuration &config,
                           const DestinationRouterPtrVariant &destination_router,
                           SourceRouter &source_router);

        void on_connect_impl() noexcept;
        void on_disconnect_impl() noexcept;
        std::size_t on_data_impl(std::string_view data, Timestamp timestamp) noexcept;
        template <typename Msg>
        void enrich_for_send_impl(Msg &msg) const noexcept;

        void on_logon(const protocol::Logon &msg) noexcept;
        void on_logout(const protocol::Logout &msg) noexcept;
        void on_heartbeat(const protocol::Heartbeat &msg) noexcept;

        void send_logon() noexcept;
        void send_logout() noexcept;
        void send_heartbeat() noexcept;

        SessionRejectInfo validate_logon(const protocol::Logon &msg) noexcept;
        SessionRejectInfo validate_logout(const protocol::Logout &msg) noexcept;
        SessionRejectInfo validate_heartbeat(const protocol::Heartbeat &msg) noexcept;

    private:
        void disconnect() noexcept;

        /* SessionStateMachine */
        SequenceStore sequence_store_;
    };

    template <typename Msg>
    inline void ProtocolA::enrich_for_send_impl(Msg &msg) const noexcept
    {
        msg.msg().header.seq_no = const_cast<ProtocolA *>(this)->sequence_store_.next_out_sequence_number();
    }
}