#pragma once

#include <framework/session/Session.h>
#include <framework/message/Message.h>
#include <framework/utility/RejectInfo.h>
#include <framework/transform/DestinationTransform.h>
#include <framework/router/SourceRouter.h>
#include <framework/application_dependency/Validators.h>
#include <string_view>
#include <type_traits>

namespace hyper::framework
{
    /* Destination-side application message session abstraction. */
    /* - Receive messages from the Destination protocol and forward them to the router */
    /* - Receive messages from the Source and forward them to the transport */

    template <typename SessionImpl, typename SourceMsg, typename DestinationMsg>

    concept DestinationSessionInf = requires(SessionImpl ds,
                                             SourceMsg src_msg, DestinationMsg dst_msg,
                                             RejectInfo reject_info) {
        { ds.on_message_from_peer_impl(src_msg) } -> std::same_as<RejectInfo>;
        { ds.on_message_from_transport_impl(src_msg) } -> std::same_as<void>;
        { ds.encode_message_to_destination_impl(src_msg, dst_msg) } -> std::same_as<RejectInfo>;
        { ds.decode_message_from_destination_impl(dst_msg, src_msg) } -> std::same_as<RejectInfo>;
        { ds.reject_message_from_transport_impl(src_msg, reject_info) } -> std::same_as<void>;
        { ds.update_destination_routing_info_impl(src_msg) } -> std::same_as<void>;
    };

    template <typename SessionImpl>
    class DestinationSession : public Session<SessionImpl>
    {
    public:
        explicit DestinationSession(Transport &transport,
                                    SourceRouter &source_router,
                                    const ValidatorPtrVariant &validator)
            : Session<SessionImpl>{transport},
              source_router_{source_router},
              validator_{validator} {}

        /* DestinationSessionInf */
        template <MessageInf Msg>
        RejectInfo on_message_from_peer(Msg &msg) noexcept;
        template <MessageInf DestinationMsg, MessageInf SourceMsg>
        void procoess_message_from_transport(DestinationMsg &dst_msg,
                                             SourceMsg &src_msg) noexcept;
        template <MessageInf SourceMsg, MessageInf DestinationMsg>
        RejectInfo procoess_message_to_transport(SourceMsg &src_msg,
                                                 DestinationMsg &dst_msg) noexcept;
        template <MessageInf Msg>
        RejectInfo validate(Msg &msg) noexcept;
        template <MessageInf Msg>
        RejectInfo transform_message_to_transport(Msg &msg) noexcept;
        template <MessageInf Msg>
        RejectInfo transform_message_from_transport(Msg &msg) noexcept;
        template <MessageInf SourceMsg, MessageInf DestinationMsg>
        RejectInfo encode_message_to_destination(SourceMsg &src_msg,
                                                 DestinationMsg &dst_msg) noexcept;
        template <MessageInf DestinationMsg, MessageInf SourceMsg>
        RejectInfo handle_message_from_transport(DestinationMsg &dst_msg,
                                                 SourceMsg &src_msg) noexcept;
        template <MessageInf DestinationMsg, MessageInf SourceMsg>
        RejectInfo decode_message_from_destination(DestinationMsg &dst_msg,
                                                   SourceMsg &src_msg) noexcept;
        template <MessageInf Msg>
        RejectInfo send_message_to_peer(Msg &msg) noexcept;
        template <MessageInf Msg>
        void reject_message_from_transport(Msg &msg, RejectInfo &reject_info) noexcept;
        template <MessageInf SourceMsg, MessageInf DestinationMsg>
        void update_destination_routing_info(SourceMsg &src_msg,
                                             DestinationMsg &dst_msg) noexcept;

    private:
        SourceRouter &source_router_;
        ValidatorPtrVariant validator_{};
        DestinationTransform destination_transform_{};
    };

    template <typename SessionImpl>
    template <MessageInf Msg>
    inline RejectInfo DestinationSession<SessionImpl>::on_message_from_peer(Msg &msg) noexcept
    {
        return this->impl().on_message_from_peer_impl(msg);
    }

    template <typename SessionImpl>
    template <MessageInf DestinationMsg, MessageInf SourceMsg>
    inline void DestinationSession<SessionImpl>::procoess_message_from_transport(DestinationMsg &dst_msg,
                                                                                 SourceMsg &src_msg) noexcept
    {
        if (auto reject_info = transform_message_from_transport(dst_msg);
            reject_info != true) [[unlikely]]
            reject_message_from_transport(dst_msg, reject_info);

        if (auto reject_info = handle_message_from_transport(dst_msg, src_msg);
            reject_info != true) [[unlikely]]
            reject_message_from_transport(dst_msg, reject_info);

        if (auto reject_info = decode_message_from_destination(dst_msg, src_msg);
            reject_info != true) [[unlikely]]
            reject_message_from_transport(dst_msg, reject_info);

        if (auto reject_info = validate(src_msg);
            reject_info != true) [[unlikely]]
            reject_message_from_transport(dst_msg, reject_info);

        if (auto reject_info = send_message_to_peer(src_msg);
            reject_info != true) [[unlikely]]
            reject_message_from_transport(dst_msg, reject_info);

        // Persist the recevied message for session recovery.
        // Publish the recevied message to other channels.
    }

    template <typename SessionImpl>
    template <MessageInf SourceMsg, MessageInf DestinationMsg>
    inline RejectInfo DestinationSession<SessionImpl>::procoess_message_to_transport(SourceMsg &src_msg,
                                                                                     DestinationMsg &dst_msg) noexcept
    {
        if (auto reject_info = validate(src_msg);
            reject_info != true) [[unlikely]]
            return reject_info;

        if (auto reject_info = encode_message_to_destination(src_msg, dst_msg);
            reject_info != true) [[unlikely]]
            return reject_info;

        if (auto reject_info = transform_message_to_transport(dst_msg);
            reject_info != true) [[unlikely]]
            return reject_info;

        if (auto reject_info = this->impl().send_message_to_transport(dst_msg);
            reject_info != true) [[unlikely]]
            return reject_info;

        /* Message sent out from the destination session */

        update_destination_routing_info(src_msg, dst_msg);

        // Persist the sent message for session recovery.
        // Publish the sent message to other channels.

        log_message_info(dst_msg);

        return RejectInfo{};
    }

    template <typename SessionImpl>
    template <MessageInf Msg>
    inline RejectInfo DestinationSession<SessionImpl>::validate(Msg &msg) noexcept
    {
        return std::visit([&msg]<typename ValidatorType>(ValidatorType &&validator)
                          { return std::forward<ValidatorType>(validator)->validate(msg); },
                          validator_);
    }

    template <typename SessionImpl>
    template <MessageInf Msg>
    inline RejectInfo DestinationSession<SessionImpl>::transform_message_to_transport(Msg &msg) noexcept
    {
        return destination_transform_.transform_message_to_transport(msg);
    }

    template <typename SessionImpl>
    template <MessageInf Msg>
    inline RejectInfo DestinationSession<SessionImpl>::transform_message_from_transport(Msg &msg) noexcept
    {
        return destination_transform_.transform_message_from_transport(msg);
    }

    template <typename SessionImpl>
    template <MessageInf SourceMsg, MessageInf DestinationMsg>
    inline RejectInfo DestinationSession<SessionImpl>::encode_message_to_destination(SourceMsg &src_msg,
                                                                                     DestinationMsg &dst_msg) noexcept
    {
        return this->impl().encode_message_to_destination_impl(src_msg, dst_msg);
    }

    template <typename SessionImpl>
    template <MessageInf DestinationMsg, MessageInf SourceMsg>
    inline RejectInfo DestinationSession<SessionImpl>::handle_message_from_transport(DestinationMsg &dst_msg, SourceMsg &src_msg) noexcept
    {
        return this->impl().handle_message_from_transport_impl(dst_msg, src_msg);
    }

    template <typename SessionImpl>
    template <MessageInf DestinationMsg, MessageInf SourceMsg>
    inline RejectInfo DestinationSession<SessionImpl>::decode_message_from_destination(DestinationMsg &dst_msg,
                                                                                       SourceMsg &src_msg) noexcept
    {
        return this->impl().decode_message_from_destination_impl(dst_msg, src_msg);
    }

    template <typename SessionImpl>
    template <MessageInf Msg>
    inline RejectInfo DestinationSession<SessionImpl>::send_message_to_peer(Msg &msg) noexcept
    {
        return source_router_.send_message_to_source(msg);
    }

    template <typename SessionImpl>
    template <MessageInf Msg>
    inline void DestinationSession<SessionImpl>::reject_message_from_transport(Msg &msg, RejectInfo &reject_info) noexcept
    {
        this->impl().reject_message_from_transport_impl(msg, reject_info);
    }

    template <typename SessionImpl>
    template <MessageInf SourceMsg, MessageInf DestinationMsg>
    inline void DestinationSession<SessionImpl>::update_destination_routing_info(SourceMsg &src_msg,
                                                                                 DestinationMsg &dst_msg) noexcept
    {
        return this->impl().update_destination_routing_info_impl(src_msg, dst_msg);
    }
}
