#pragma once

#include <variant>

#include <application/router/DestinatinRouter.h>
#include <application/router/DestinationRouterOneToOne.h>

namespace max
{
    using DestinatinRouterVarient = std::variant<DestinationRouterOneToOne>;
};