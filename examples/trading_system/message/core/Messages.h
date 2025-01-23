#pragma once

#include <framework/message/Message.h>

namespace hyper::trading
{
    using Message = hyper::framework::Message;
    using FirstEvent = hyper::framework::FirstEvent;
    using SubsequentEvent = hyper::framework::SubsequentEvent;

    /* Message interface supported by the system */

    struct NewOrderSingle : public FirstEvent
    {
    };

    struct CancelReplaceRequest : public SubsequentEvent
    {
    };

    struct CancelRequest : public SubsequentEvent
    {
    };

    struct ExecutionReport : public SubsequentEvent
    {
    };

    struct CancelReject : public SubsequentEvent
    {
    };
}