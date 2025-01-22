
#pragma once

#include <framework/factory/Factory.h>
#include <framework/config/Configuration.h>
#include <framework/application_dependency/DestinationRouterDeclarations.h>
#include <framework/application_dependency/SourceProtocolDeclarations.h>

namespace hyper::framework
{
    class SourceRouter;

    using SourceProtocolCreator =
        std::function<SourceProtocolPtrVariant(const Configuration &config,
                                               const DestinationRouterPtrVariant &destination_router,
                                               SourceRouter &source_router)>;

    using SourceProtocolFactory = Factory<SourceProtocolCreator>;

    template <typename Type>
    inline void register_source_protocol(const std::string &key)
    {
        auto &factory = SourceProtocolFactory::instance();
        factory.register_type(
            key,
            [](const Configuration &config,
               const DestinationRouterPtrVariant &destination_router,
               SourceRouter &source_router)
            {
                return std::make_shared<Type>(config, destination_router, source_router);
            });
    }
}
