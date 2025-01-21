
#pragma once

#include <framework/factory/Factory.h>
#include <framework/application_dependency/DestinationRouters.h>
#include <framework/config/Configuration.h>

namespace hyper::framework
{
    class SourceRouter;

    using DestinationRouterCreator =
        std::function<DestinationRouterPtrVarient(const Configuration &config,
                                                  const DestinationProtocolByUid &destinations)>;

    using DestinationRouterFactory = Factory<DestinationRouterCreator>;

    template <typename Type>
    inline void register_destination_router(const std::string &key)
    {
        auto &factory = DestinationRouterFactory::instance();
        factory.register_type(
            key,
            [](const Configuration &config, const DestinationProtocolByUid &destinations)
            {
                return std::make_shared<Type>(config, destinations);
            });
    }

    inline void register_all_destination_routers()
    {
        register_destination_router<DestinationRouterOneToOne>("OneToOne");
        register_destination_router<DestinationRouterRoundRobin>("RoundRobin");
    }
}
