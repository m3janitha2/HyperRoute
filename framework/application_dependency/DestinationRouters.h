#pragma once

#include <variant>

#include <framework/router/DestinationRouter.h>
#include <framework/router/DestinationRouterOneToOne.h>
#include <framework/router/DestinationRouterRoundRobin.h>

namespace hyper
{
    using DestinationRouterPtrVarient = std::variant<framework::DestinationRouterOneToOne *,
                                                     framework::DestinationRouterRoundRobin *>;
};