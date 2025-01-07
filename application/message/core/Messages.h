#pragma once

#include <framework/message/Message.h>

/* Example of a message schema for a trading system */
namespace hyper::trading
{
    using Message = hyper::framework::message::Message;
    using FirstEvent = hyper::framework::message::FirstEvent;
    using SubsequentEvent = hyper::framework::message::SubsequentEvent;

    /* Message types supported by the system */
    struct NewOrderSingle : public Message, FirstEvent
    {
    };

    struct CancelReplaceRequest : public Message, SubsequentEvent
    {
    };

    struct CancelRequest : public Message, SubsequentEvent
    {
    };

    struct ExecutionReport : public Message, SubsequentEvent
    {
    };

    struct CancelReject : public Message, SubsequentEvent
    {
    };
}