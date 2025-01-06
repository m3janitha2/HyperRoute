#pragma once

#include <variant>

#include <framework/router/DestinationRouter.h>
#include <framework/router/DestinationRouterOneToOne.h>
#include <framework/router/DestinationRouterOneToMany.h>

namespace max
{
    using DestinationRouterPtrVarient = std::variant<framework::DestinationRouterOneToOne *,
                                                     framework::DestinationRouterOneToMany *>;
};