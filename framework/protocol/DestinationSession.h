#pragma once

#include <framework/utility/CrtpBase.h>
#include <framework/protocol/RejectInfo.h>
#include <framework/enricher/DestinationEnricher.h>
#include <framework/router/SourceRouter.h>
#include <framework/transport/Transport.h>
#include <application/validator/Validator.h>

namespace max::framework
{
    class DestinationSessionInfo;

    template <typename SessionImpl>
    class DestinationSession : public CrtpBase<SessionImpl>
    {
    public:
        explicit DestinationSession(SourceRouter &source_router,
                                    Transport &transport)
            : source_router_{source_router},
              transport_{transport} {}

        DestinationSession(const DestinationSession &) = delete;
        DestinationSession &operator=(const DestinationSession &) = delete;

        /* TransportEvents */
        void on_connect() { this->impl().on_connect_impl(); }
        void on_disconnect() { this->impl().on_disconnect_impl(); }

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

    private:
        Validator validator_{};
        DestinationEnricher destination_enricher_{};
        SourceRouter &source_router_;
        Transport &transport_;
    };

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

        if (auto reject_info = validator_.validate(src_msg);
            reject_info != true) [[unlikely]]
            rejecet_message_from_transport(dst_msg, reject_info);

        if (auto reject_info = send_message_to_peer(src_msg);
            reject_info != true) [[unlikely]]
            rejecet_message_from_transport(dst_msg, reject_info);

        // Persist DestinationSession message
        // Publish DestinationSession message for downstream
    }

    template <typename SessionImpl>
    template <typename SourceMsg, typename DestinationMsg>
    inline RejectInfo DestinationSession<SessionImpl>::procoess_message_to_transport(SourceMsg &src_msg,
                                                                                     DestinationMsg &dst_msg) noexcept
    {
        if (auto reject_info = validator_.validate(src_msg); reject_info != true) [[unlikely]]
            return reject_info;

        if (auto reject_info = encode_message_to_destination(src_msg, dst_msg); reject_info != true) [[unlikely]]
            return reject_info;

        if (auto reject_info = enrich_message_to_transport(dst_msg); reject_info != true) [[unlikely]]
            return reject_info;

        if (auto reject_info = send_message_to_transport(dst_msg); reject_info != true) [[unlikely]]
            return reject_info;

        // Persist DestinationSession message
        // Publish DestinationSession message for downstream

        std::cout << "sent message to transport: " << dst_msg
                  << " in:" << dst_msg.in_time()
                  << " out:" << dst_msg.out_time()
                  << " latency_in_ns:" << dst_msg.latency_in_ns() << std::endl;
        return RejectInfo{};
    }

    template <typename SessionImpl>
    template <typename Msg>
    inline RejectInfo DestinationSession<SessionImpl>::enrich_message_to_transport(Msg &msg) noexcept
    {
        // std::cout << "enrich_message_to_transport:" << msg.msg() << std::endl;
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
        msg.update_out_time();
        std::string_view data{reinterpret_cast<char *>(&msg), sizeof(msg)};
        return transport_.send_data(data);
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