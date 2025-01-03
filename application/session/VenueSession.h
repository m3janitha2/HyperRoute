#pragma once

#include <application/util/CrtpWrapper.h>
#include <application/session/RejectInfo.h>
#include <application/enricher/VenueEnricher.h>
#include <application/router/ReverseRouter.h>
#include <application/validator/Validator.h>
#include <application/transport/Transport.h>

namespace max
{
    template <typename SessionImpl>
    class VenueSessionBase : public core::CrtpWrapper<SessionImpl>
    {
    public:
        explicit VenueSessionBase(ReverseRouter &reverse_router,
                                  Transport &transport)
            : reverse_router_{reverse_router},
              transport_{transport} {}

        /* TransportEvents */
        void on_connect() { this->impl().on_connect_impl(); }
        void on_disconnect() { this->impl().on_disconnect_impl(); }

        /* messages from client to venue */
        template <typename Msg>
        RejectInfo on_client_message(Msg &msg);
        template <typename Msg>
        void on_message(Msg &msg) { this->impl().on_message_impl(msg); }

        /* venue session internal */
        template <typename Msg>
        void procoess_message_from_venue(Msg &msg);
        template <typename Msg>
        RejectInfo procoess_message_to_venue(Msg &msg);
        template <typename Msg>
        RejectInfo enrich_message_to_venue(Msg &msg);
        template <typename Msg>
        RejectInfo enrich_message_from_venue(Msg &msg);
        template <typename ClientMsg>
        auto encode_message_to_venue(ClientMsg &msg) { return this->impl().encode_message_to_venue_impl(msg); }
        template <typename VenueMsg>
        auto decode_message_from_venue(VenueMsg &msg) { return this->impl().decode_message_from_venue_impl(msg); }
        template <typename Msg>
        RejectInfo send_message_to_venue(Msg &msg);
        template <typename Msg>
        RejectInfo send_message_to_client(Msg &msg);
        template <typename Msg>
        void reject_venue_message(Msg &msg, RejectInfo &reject_info) { this->impl().reject_venue_message_impl(msg, reject_info); }

    private:
        Validator validator_{};
        VenueEnricher venue_enricher_{};
        ReverseRouter &reverse_router_;
        Transport &transport_;
    };

    template <typename SessionImpl>
    template <typename Msg>
    RejectInfo VenueSessionBase<SessionImpl>::on_client_message(Msg &msg)
    {
        return procoess_message_to_venue(msg);
    }

    template <typename SessionImpl>
    template <typename Msg>
    RejectInfo VenueSessionBase<SessionImpl>::procoess_message_to_venue(Msg &client_msg)
    {
        if (auto reject_info = validator_.validate(client_msg); reject_info != true)
            return reject_info;

        auto [reject, venue_msg] = encode_message_to_venue(client_msg);
        if (reject != true)
            return reject;

        if (auto reject_info = enrich_message_to_venue(venue_msg); reject_info != true)
            return reject_info;

        if (auto reject_info = send_message_to_venue(venue_msg); reject_info != true)
            return reject_info;

        // persist venue message
        // publish venue message for downstream
        return RejectInfo{};
    }

    template <typename SessionImpl>
    template <typename Msg>
    void VenueSessionBase<SessionImpl>::procoess_message_from_venue(Msg &venue_msg)
    {
        if (auto reject_info = enrich_message_from_venue(venue_msg); reject_info != true)
            reject_venue_message(venue_msg, reject_info);

        auto [reject, client_msg] = decode_message_from_venue(venue_msg);
        if (reject != true)
            reject_venue_message(venue_msg, reject);

        if (auto reject_info = validator_.validate(client_msg); reject_info != true)
            reject_venue_message(venue_msg, reject_info);

        if (auto reject_info = send_message_to_client(client_msg); reject_info != true)
            reject_venue_message(venue_msg, reject_info);

        // persist venue message
        // publish venue message for downstream
    }

    template <typename SessionImpl>
    template <typename Msg>
    RejectInfo VenueSessionBase<SessionImpl>::enrich_message_to_venue(Msg &msg)
    {
        std::cout << "enrich_message_to_venue:" << msg.msg() << std::endl;
        return venue_enricher_.enrich_message_to_venue(msg);
    }

    template <typename SessionImpl>
    template <typename Msg>
    RejectInfo VenueSessionBase<SessionImpl>::enrich_message_from_venue(Msg &msg)
    {
        return venue_enricher_.enrich_message_from_venue(msg);
    }

    template <typename SessionImpl>
    template <typename Msg>
    RejectInfo VenueSessionBase<SessionImpl>::send_message_to_venue(Msg &msg)
    {
        std::cout << "send_message_to_venue:" << msg << std::endl;
        std::string_view data{reinterpret_cast<char *>(&msg), sizeof(msg)};
        return transport_.send_data(data);
    }

    template <typename SessionImpl>
    template <typename Msg>
    RejectInfo VenueSessionBase<SessionImpl>::send_message_to_client(Msg &msg)
    {
        return reverse_router_.send_message_to_client(msg);
    }
}