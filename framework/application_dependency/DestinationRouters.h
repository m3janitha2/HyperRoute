#pragma once

#include <variant>
#include <memory>
#include <framework/router/DestinationRouter.h>
#include <framework/router/DestinationRouterOneToOne.h>
#include <framework/router/DestinationRouterRoundRobin.h>

namespace hyper
{
    using DestinationRouterPtrVariant = std::variant<std::shared_ptr<framework::DestinationRouterOneToOne>,
                                                     std::shared_ptr<framework::DestinationRouterRoundRobin>>;
};