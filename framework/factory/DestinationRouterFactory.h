
#pragma once

#include <framework/factory/Factory.h>
#include <framework/config/Configuration.h>
#include <framework/router/DestinationRouter.h>
#include <framework/application_dependency/DestinationRouterDeclarations.h>

namespace hyper::framework
{
    using DestinationRouterCreator =
        std::function<DestinationRouterPtrVariant(const Configuration &config,
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
}
