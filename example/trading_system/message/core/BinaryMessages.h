#pragma once

#include <framework/message/BinaryMessage.h>
#include <example/trading_system/message/core/Messages.h>

namespace hyper::trading::binary
{
    /* Binary messages supported by the system */
    
    template <typename Msg>
    using BinaryMessage = framework::message::binary::BinaryMessage<Msg>;

    template <typename Msg>
    struct NewOrderSingle : public BinaryMessage<Msg>, trading::NewOrderSingle
    {
        explicit constexpr NewOrderSingle(Msg &msg) noexcept
            : BinaryMessage<Msg>{msg} {}
        explicit constexpr NewOrderSingle(std::string_view data) noexcept
            : BinaryMessage<Msg>{data} {}
    };

    template <typename Msg>
    struct CancelReplaceRequest : public BinaryMessage<Msg>, trading::CancelReplaceRequest
    {
        explicit constexpr CancelReplaceRequest(Msg &msg) noexcept
            : BinaryMessage<Msg>{msg} {}
        explicit constexpr CancelReplaceRequest(std::string_view data) noexcept
            : BinaryMessage<Msg>{data} {}
    };

    template <typename Msg>
    struct CancelRequest : public BinaryMessage<Msg>, trading::CancelRequest
    {
        explicit constexpr CancelRequest(Msg &msg) noexcept
            : BinaryMessage<Msg>{msg} {}
        explicit constexpr CancelRequest(std::string_view data) noexcept
            : BinaryMessage<Msg>{data} {}
    };

    template <typename Msg>
    struct ExecutionReport : public BinaryMessage<Msg>, trading::ExecutionReport
    {
        explicit constexpr ExecutionReport(Msg &msg) noexcept
            : BinaryMessage<Msg>{msg} {}
        explicit constexpr ExecutionReport(std::string_view data) noexcept
            : BinaryMessage<Msg>{data} {}
    };

    template <typename Msg>
    struct CancelReject : public BinaryMessage<Msg>, trading::CancelReject
    {
        explicit constexpr CancelReject(Msg &msg) noexcept
            : BinaryMessage<Msg>{msg} {}
        explicit constexpr CancelReject(std::string_view data) noexcept
            : BinaryMessage<Msg>{data} {}
    };
}