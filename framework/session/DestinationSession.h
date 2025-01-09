#pragma once

#include <framework/utility/CrtpBase.h>
#include <framework/utility/RejectInfo.h>
#include <framework/enricher/DestinationEnricher.h>
#include <framework/router/SourceRouter.h>
#include <framework/transport/Transport.h>
#include <framework/application_dependency/Validators.h>

namespace hyper::framework
{
    /* Destination-side application message session abstraction. */
    /* - Receive messages from the Destination protocol and forward them to the router */
    /* - Receive messages from the Source and forward them to the transport */
    template <typename SessionImpl>
    class DestinationSession : public CrtpBase<SessionImpl>
    {
    public:
        explicit DestinationSession(SourceRouter &source_router,
                                    Transport &transport,
                                    ValidatorPtrVarient &validator)
            : source_router_{source_router},
              transport_{transport},
              validator_(validator) {}

        DestinationSession(const DestinationSession &) = delete;
        DestinationSession &operator=(const DestinationSession &) = delete;

        /* TransportEvents */
        void on_connect() noexcept;
        void on_disconnect() noexcept;

        template <typename Msg>
        RejectInfo on_message_from_peer(Msg &msg) noexcept;
        template <typename Msg>
        void on_message_from_transport(Msg &msg) noexcept;
        template <typename DestinationMsg, typename SourceMsg>
        void procoess_message_from_transport(DestinationMsg &dst_msg,
                                             SourceMsg &src_msg) noexcept;
        template <typename SourceMsg, typename DestinationMsg>
        RejectInfo procoess_message_to_transport(SourceMsg &src_msg,
                                                 DestinationMsg &dst_msg) noexcept;
        template <typename Msg>
        RejectInfo validate(Msg &msg) noexcept;
        template <typename Msg>
        RejectInfo enrich_message_to_transport(Msg &msg) noexcept;
        template <typename Msg>
        RejectInfo enrich_message_from_transport(Msg &msg) noexcept;
        template <typename SourceMsg, typename DestinationMsg>
        RejectInfo encode_message_to_destination(SourceMsg &src_msg,
                                                 DestinationMsg &dst_msg) noexcept;
        template <typename DestinationMsg, typename SourceMsg>
        RejectInfo decode_message_from_destination(DestinationMsg &dst_msg,
                                                   SourceMsg &src_msg) noexcept;
        template <typename Msg>
        RejectInfo send_message_to_transport(Msg &msg) noexcept;
        template <typename Msg>
        RejectInfo send_message_to_peer(Msg &msg) noexcept;
        template <typename Msg>
        void rejecet_message_from_transport(Msg &msg, RejectInfo &reject_info) noexcept;

        [[nodiscard]] constexpr bool is_connected() const noexcept { return connected_; }

    private:
        ValidatorPtrVarient &validator_{};
        DestinationEnricher destination_enricher_{};
        SourceRouter &source_router_;
        Transport &transport_;
        bool connected_{true};
    };

    template <typename SessionImpl>
    inline void DestinationSession<SessionImpl>::on_connect() noexcept
    {
        this->impl().on_connect_impl();
    }

    template <typename SessionImpl>
    inline void DestinationSession<SessionImpl>::on_disconnect() noexcept
    {
        this->impl().on_disconnect_impl();
    }

    template <typename SessionImpl>
    template <typename Msg>
    inline RejectInfo DestinationSession<SessionImpl>::on_message_from_peer(Msg &msg) noexcept
    {
        return this->impl().on_message_from_peer_impl(msg);
    }

    template <typename SessionImpl>
    template <typename Msg>
    inline void DestinationSession<SessionImpl>::on_message_from_transport(Msg &msg) noexcept
    {
        this->impl().on_message_from_transport_impl(msg);
    }

    template <typename SessionImpl>
    template <typename DestinationMsg, typename SourceMsg>
    inline void DestinationSession<SessionImpl>::procoess_message_from_transport(DestinationMsg &dst_msg,
                                                                                 SourceMsg &src_msg) noexcept
    {
        if (auto reject_info = enrich_message_from_transport(dst_msg);
            reject_info != true) [[unlikely]]
            rejecet_message_from_transport(dst_msg, reject_info);

        if (auto reject_info = decode_message_from_destination(dst_msg, src_msg);
            reject_info != true) [[unlikely]]
            rejecet_message_from_transport(dst_msg, reject_info);

        if (auto reject_info = validate(src_msg);
            reject_info != true) [[unlikely]]
            rejecet_message_from_transport(dst_msg, reject_info);

        if (auto reject_info = send_message_to_peer(src_msg);
            reject_info != true) [[unlikely]]
            rejecet_message_from_transport(dst_msg, reject_info);

        // Persist the recevied message for session recovery.
        // Publish the recevied message to other channels.
    }

    template <typename SessionImpl>
    template <typename SourceMsg, typename DestinationMsg>
    inline RejectInfo DestinationSession<SessionImpl>::procoess_message_to_transport(SourceMsg &src_msg,
                                                                                     DestinationMsg &dst_msg) noexcept
    {
        if (auto reject_info = validate(src_msg); reject_info != true) [[unlikely]]
            return reject_info;

        if (auto reject_info = encode_message_to_destination(src_msg, dst_msg); reject_info != true) [[unlikely]]
            return reject_info;

        if (auto reject_info = enrich_message_to_transport(dst_msg); reject_info != true) [[unlikely]]
            return reject_info;

        if (auto reject_info = send_message_to_transport(dst_msg); reject_info != true) [[unlikely]]
            return reject_info;

        // Persist the sent message for session recovery.
        // Publish the sent message to other channels.

        log_message_info(dst_msg);

        return RejectInfo{};
    }

    template <typename SessionImpl>
    template <typename Msg>
    inline RejectInfo DestinationSession<SessionImpl>::validate(Msg &msg) noexcept
    {
        return std::visit([&msg]<typename ValidatorType>(ValidatorType &&validator)
                          { return std::forward<ValidatorType>(validator)->validate(msg); },
                          validator_);
    }

    template <typename SessionImpl>
    template <typename Msg>
    inline RejectInfo DestinationSession<SessionImpl>::enrich_message_to_transport(Msg &msg) noexcept
    {
        return destination_enricher_.enrich_message_to_transport(msg);
    }

    template <typename SessionImpl>
    template <typename Msg>
    inline RejectInfo DestinationSession<SessionImpl>::enrich_message_from_transport(Msg &msg) noexcept
    {
        return destination_enricher_.enrich_message_from_transport(msg);
    }

    template <typename SessionImpl>
    template <typename SourceMsg, typename DestinationMsg>
    inline RejectInfo DestinationSession<SessionImpl>::encode_message_to_destination(SourceMsg &src_msg,
                                                                                     DestinationMsg &dst_msg) noexcept
    {
        return this->impl().encode_message_to_destination_impl(src_msg, dst_msg);
    }

    template <typename SessionImpl>
    template <typename DestinationMsg, typename SourceMsg>
    inline RejectInfo DestinationSession<SessionImpl>::decode_message_from_destination(DestinationMsg &dst_msg,
                                                                                       SourceMsg &src_msg) noexcept
    {
        return this->impl().decode_message_from_destination_impl(dst_msg, src_msg);
    }

    template <typename SessionImpl>
    template <typename Msg>
    inline RejectInfo DestinationSession<SessionImpl>::send_message_to_transport(Msg &msg) noexcept
    {
        msg.update_out_timestamp();
        return transport_.send_data(msg.data());
    }

    template <typename SessionImpl>
    template <typename Msg>
    inline RejectInfo DestinationSession<SessionImpl>::send_message_to_peer(Msg &msg) noexcept
    {
        return source_router_.send_message_to_source(msg);
    }

    template <typename SessionImpl>
    template <typename Msg>
    inline void DestinationSession<SessionImpl>::rejecet_message_from_transport(Msg &msg, RejectInfo &reject_info) noexcept
    {
        this->impl().rejecet_message_from_transport_impl(msg, reject_info);
    }
}