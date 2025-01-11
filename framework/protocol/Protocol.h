#pragma once

#include <framework/utility/CrtpBase.h>
#include <framework/utility/RejectInfo.h>
#include <framework/transport/Transport.h>
#include <framework/sequence_store/SequenceStore.h>
#include <cstdint>
#include <string_view>

namespace hyper::framework
{
    /* Protocol abstraction */
    /* - Receive data from the Transport */
    /* - Handle the protocol session */
    /* - Dispatch application messages to the Session */
    template <typename ProtocolImpl, typename Session>
    class Protocol : public CrtpBase<ProtocolImpl>
    {
    public:
        template <typename... Args>
        explicit Protocol(Args &&...args)
            : session_{transport_, std::forward<Args>(args)...}
        {
            static_assert(TransportCallbacksInf<Protocol<ProtocolImpl, Session>>,
                          "Protocol does not satisfy TransportCallbacksInf");
        }

        Protocol(const Protocol &) = delete;
        Protocol &operator=(const Protocol &) = delete;

        /* TransportCallbacks */
        void on_connect() noexcept;
        void on_disconnect() noexcept;
        std::size_t on_data(std::string_view data) noexcept;

        RejectInfo connect();
        RejectInfo disconnect();
        [[nodiscard]] constexpr bool is_connected() const noexcept { return connected_; }

        template <typename Msg>
        RejectInfo send_to_transport(Msg &msg) noexcept;
        template <typename Msg>
        void persist_protocol_message(Msg &msg) {}

        [[nodiscard]] constexpr Transport &transport() noexcept { return transport_; }
        [[nodiscard]] constexpr const Transport &transport() const noexcept { return transport_; }

        [[nodiscard]] constexpr const Session &session() const noexcept { return session_; }
        [[nodiscard]] constexpr Session &session() noexcept { return session_; }

    private:
        TransportCallbacks transport_callbacks{[this]()
                                               { on_connect(); },
                                               [this]()
                                               { on_disconnect(); },
                                               [this](std::string_view data)
                                               { return on_data(data); }};
        Transport transport_{transport_callbacks};
        Session session_;
        SequenceStore<std::uint64_t> sequence_store_{};
        bool connected_{false};
        /* todox: HeatbeatTimer producer_ */
        /* todox: HeatbeatTimer receiver_ */
        /* todox: PersistStore msg_stroe_ */
    };

    template <typename ProtocolImpl, typename Session>
    inline void Protocol<ProtocolImpl, Session>::on_connect() noexcept
    {
        this->impl().on_connect_impl();
    }

    template <typename ProtocolImpl, typename Session>
    inline void Protocol<ProtocolImpl, Session>::on_disconnect() noexcept
    {
        this->impl().on_disconnect_impl();
    }

    template <typename ProtocolImpl, typename Session>
    inline std::size_t Protocol<ProtocolImpl, Session>::on_data(std::string_view data) noexcept
    {
        /* todox: reschedule heatbeat receiver */
        return this->impl().on_data_impl(data);
    }

    template <typename ProtocolImpl, typename Session>
    inline RejectInfo Protocol<ProtocolImpl, Session>::connect()
    {
        return RejectInfo{};
    }

    template <typename ProtocolImpl, typename Session>
    inline RejectInfo Protocol<ProtocolImpl, Session>::disconnect()
    {
        return RejectInfo{};
    }

    template <typename ProtocolImpl, typename Session>
    template <typename Msg>
    inline RejectInfo Protocol<ProtocolImpl, Session>::send_to_transport(Msg &msg) noexcept
    {
        /* todox: reschedule heatbeat producer_ */
        std::string_view data{reinterpret_cast<char *>(&msg), sizeof(msg)};
        return transport_.send_data(data);
    }
}