#pragma once

#include <variant>
#include <memory>

namespace hyper
{
    namespace framework
    {
        class DestinationRouterOneToOne;
        class DestinationRouterRoundRobin;
    }

    using DestinationRouterPtrVariant = std::variant<
        std::shared_ptr<framework::DestinationRouterOneToOne>,
        std::shared_ptr<framework::DestinationRouterRoundRobin>>;
}