#pragma once

#include <variant>

#include <application/router/ForwardRouterBase.h>
#include <application/router/ForwardRouterOneToOne.h>

namespace max
{
    using ForwardRouterVarient = std::variant<ForwardRouterOneToOne>;
};