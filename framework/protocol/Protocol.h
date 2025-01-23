#pragma once

#include <framework/utility/CrtpBase.h>
#include <framework/utility/RejectInfo.h>
#include <framework/transport/Transport.h>
#include <framework/sequence_store/SequenceStore.h>
#include <framework/config/Configuration.h>
#include <cstdint>
#include <string_view>
#include <type_traits>

namespace hyper::framework
{
    /* Protocol abstraction */
    /* - Receive data from the Transport */
    /* - Handle the protocol session */
    /* - Dispatch application messages to the Session */

    template <typename ProtocolImpl>
    concept ProtocolInf = requires(ProtocolImpl p, std::string_view data) {
        { p.impl().on_connect_impl() } -> std::same_as<void>;
        { p.impl().on_disconnect_impl() } -> std::same_as<void>;
        { p.impl().on_data_impl(data) } -> std::same_as<std::size_t>;
    };

    template <typename ProtocolImpl, typename Session>
    class Protocol : public CrtpBase<ProtocolImpl>
    {
    public:
        template <typename... Args>
        explicit Protocol(const Configuration &config, Args &&...args)
            : config_(config),
              transport_{TransportCallbacks{
                  [this]() noexcept
                  { on_connect(); },
                  [this]() noexcept
                  { on_disconnect(); },
                  [this](std::string_view data) noexcept
                  { return on_data(data); }}},
              session_{transport_, std::forward<Args>(args)...}
        {
            static_assert(TransportCallbackInf<Protocol<ProtocolImpl, Session>>,
                          "Protocol does not satisfy TransportCallbackInf");
            static_assert(ProtocolInf<Protocol<ProtocolImpl, Session>>,
                          "Protocol does not satisfy ProtocolInf");
            load(config);
        }

        Protocol(const Protocol &) = delete;
        Protocol &operator=(const Protocol &) = delete;

        void load(const Configuration &config);

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

        [[nodiscard]] constexpr const std::string &name() const noexcept { return name_; }

    private:
        const Configuration &config_;
        std::size_t id_{0};
        std::string name_{};
        Transport transport_;
        Session session_;
        SequenceStore<std::uint64_t> sequence_store_{};
        bool connected_{false};

        /* todox: HeatbeatTimer producer_ */
        /* todox: HeatbeatTimer receiver_ */
        /* todox: PersistStore msg_store_ */
    };

    template <typename ProtocolImpl, typename Session>
    inline void Protocol<ProtocolImpl, Session>::load(const Configuration &config)
    {
        id_ = config.get<std::size_t>("id");
        name_ = config.get<std::string>("name");
    }

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