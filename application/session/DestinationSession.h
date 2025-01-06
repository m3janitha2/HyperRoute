#pragma once

#include <application/util/CrtpWrapper.h>
#include <application/session/RejectInfo.h>
#include <application/enricher/VenueEnricher.h>
#include <application/router/ReverseRouter.h>
#include <application/validator/Validator.h>
#include <application/transport/Transport.h>

namespace max
{
    class DestinationSessionInfo;

    template <typename SessionImpl>
    class DestinationSessionBase : public core::CrtpWrapper<SessionImpl>
    {
    public:
        explicit DestinationSessionBase(ReverseRouter &reverse_router,
                                        Transport &transport)
            : reverse_router_{reverse_router},
              transport_{transport} {}

        // DestinationSessionBase(const DestinationSessionBase &) = delete;

        /* TransportEvents */
        void on_connect() { this->impl().on_connect_impl(); }
        void on_disconnect() { this->impl().on_disconnect_impl(); }

        template <typename Msg>
        RejectInfo on_message_from_peer(Msg &msg) { return this->impl().on_message_from_peer_impl(msg); }
        template <typename Msg>
        void on_message_from_transport(Msg &msg) { this->impl().on_message_from_transport_impl(msg); }
        template <typename DestinationMsg, typename SourceMsg>
        void procoess_message_from_transport(DestinationMsg &destination_msg, SourceMsg &source_msg);
        template <typename SourceMsg, typename DestinationMsg>
        RejectInfo procoess_message_to_transport(SourceMsg &source_msg, DestinationMsg &destination_msg);
        template <typename Msg>
        RejectInfo enrich_message_to_transport(Msg &msg);
        template <typename Msg>
        RejectInfo enrich_message_from_transport(Msg &msg);
        template <typename SourceMsg, typename DestinationMsg>
        auto encode_message_to_destination(SourceMsg &source_msg, DestinationMsg &destination_msg) { return this->impl().encode_message_to_destination_impl(source_msg, destination_msg); }
        template <typename DestinationMsg, typename SourceMsg>
        auto decode_message_from_destination(DestinationMsg &destination_msg, SourceMsg &source_msg) { return this->impl().decode_message_from_destination_impl(destination_msg, source_msg); }
        template <typename Msg>
        RejectInfo send_message_to_transport(Msg &msg);
        template <typename Msg>
        RejectInfo send_message_to_peer(Msg &msg);
        template <typename Msg>
        void rejecet_message_from_transport(Msg &msg, RejectInfo &reject_info) { this->impl().rejecet_message_from_transport_impl(msg, reject_info); }

    private:
        Validator validator_{};
        VenueEnricher venue_enricher_{};
        ReverseRouter &reverse_router_;
        Transport &transport_;
    };

    template <typename SessionImpl>
    template <typename SourceMsg, typename DestinationMsg>
    RejectInfo DestinationSessionBase<SessionImpl>::procoess_message_to_transport(SourceMsg &source_msg, DestinationMsg &destination_msg)
    {
        if (auto reject_info = validator_.validate(source_msg); reject_info != true) [[unlikely]]
            return reject_info;

        if (auto reject_info = encode_message_to_destination(source_msg, destination_msg); reject_info != true) [[unlikely]]
            return reject_info;

        if (auto reject_info = enrich_message_to_transport(destination_msg); reject_info != true) [[unlikely]]
            return reject_info;

        if (auto reject_info = send_message_to_transport(destination_msg); reject_info != true) [[unlikely]]
            return reject_info;

        // Persist DestinationSession message
        // Publish DestinationSession message for downstream
        return RejectInfo{};
    }

    template <typename SessionImpl>
    template <typename DestinationMsg, typename SourceMsg>
    void DestinationSessionBase<SessionImpl>::procoess_message_from_transport(DestinationMsg &destination_msg, SourceMsg &source_msg)
    {
        if (auto reject_info = enrich_message_from_transport(destination_msg); reject_info != true) [[unlikely]]
            rejecet_message_from_transport(destination_msg, reject_info);

        if (auto reject_info = decode_message_from_destination(destination_msg, source_msg); reject_info != true) [[unlikely]]
            rejecet_message_from_transport(destination_msg, reject_info);

        if (auto reject_info = validator_.validate(source_msg); reject_info != true) [[unlikely]]
            rejecet_message_from_transport(destination_msg, reject_info);

        if (auto reject_info = send_message_to_peer(source_msg); reject_info != true) [[unlikely]]
            rejecet_message_from_transport(destination_msg, reject_info);

        // persist DestinationSession message
        // publish DestinationSession message for downstream
    }

    template <typename SessionImpl>
    template <typename Msg>
    RejectInfo DestinationSessionBase<SessionImpl>::enrich_message_to_transport(Msg &msg)
    {
        std::cout << "enrich_message_to_transport:" << msg.msg() << std::endl;
        return venue_enricher_.enrich_message_to_transport(msg);
    }

    template <typename SessionImpl>
    template <typename Msg>
    RejectInfo DestinationSessionBase<SessionImpl>::enrich_message_from_transport(Msg &msg)
    {
        return venue_enricher_.enrich_message_from_transport(msg);
    }

    template <typename SessionImpl>
    template <typename Msg>
    RejectInfo DestinationSessionBase<SessionImpl>::send_message_to_transport(Msg &msg)
    {
        std::cout << "send_message_to_transport:" << msg << std::endl;
        std::string_view data{reinterpret_cast<char *>(&msg), sizeof(msg)};
        return transport_.send_data(data);
    }

    template <typename SessionImpl>
    template <typename Msg>
    RejectInfo DestinationSessionBase<SessionImpl>::send_message_to_peer(Msg &msg)
    {
        return reverse_router_.send_message_to_source(msg);
    }
}