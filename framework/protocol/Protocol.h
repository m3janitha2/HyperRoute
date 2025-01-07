#pragma once

#include <framework/utility/CrtpBase.h>
#include <framework/protocol/RejectInfo.h>
#include <framework/transport/Transport.h>
#include <cstdint>
#include <string_view>

namespace hyper::framework
{
    /* todox: persisted sequence number */
    struct SequenceStore
    {
        std::uint64_t in_sequence_number_{0};
        std::uint64_t out_sequence_number_{0};
    };

    template <typename ProtocolImpl>
    class Protocol : public CrtpBase<ProtocolImpl>
    {
    public:
        explicit Protocol() = default;

        void on_connect() { this->impl().on_connect_impl(); }
        void on_disconnect() { this->impl().on_disconnect_impl(); }
        std::size_t on_data(std::string_view data);

        [[nodiscard]] constexpr Transport &transport() noexcept { return transport_; }
        [[nodiscard]] constexpr const Transport &transport() const noexcept { return transport_; }

        [[nodiscard]] constexpr bool is_connected() const noexcept { return connected_; }

        template <typename Msg>
        RejectInfo send_to_transport(Msg &msg);

        template <typename Msg>
        void persist_session_message(Msg &msg) {}
        template <typename Msg>
        void publish_session_message(Msg &msg) {}

        TransportCallbacks transport_callbacks{[this]()
                                               { on_connect(); },
                                               [this]()
                                               { on_disconnect(); },
                                               [this](std::string_view data)
                                               { return on_data(data); }};
        Transport transport_{transport_callbacks};
        SequenceStore sequence_store_{};
        bool connected_{false};
        /* todox: HeatbeatTimer producer_ */
        /* todox: HeatbeatTimer receiver_ */
        /* todox: MessageStore msg_stroe_ */
    };

    template <typename ProtocolImpl>
    inline std::size_t Protocol<ProtocolImpl>::on_data(std::string_view data)
    {
        /* todox: reschedule heatbeat receiver */
        return this->impl().on_data_impl(data);
    }

    template <typename ProtocolImpl>
    template <typename Msg>
    inline RejectInfo Protocol<ProtocolImpl>::send_to_transport(Msg &msg)
    {
        /* todox: reschedule heatbeat producer_ */
        std::string_view data{reinterpret_cast<char *>(&msg), sizeof(msg)};
        return transport_.send_data(data);
    }
}