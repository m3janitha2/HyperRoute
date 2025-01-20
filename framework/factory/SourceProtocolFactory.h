
#pragma once

#include <framework/factory/Factory.h>
#include <framework/application_dependency/SourceProtocols.h>
#include <framework/config/Configuration.h>

namespace hyper::framework
{
    class SourceRouter;

    using SourceProtocolCreator =
        std::function<SourceProtocolPtrVarient(const Configuration &config,
                                               const DestinationRouterPtrVarient &destination_router,
                                               const SourceRouter &source_router)>;

    using SourceProtocolFactory =
        Factory<SourceProtocolPtrVarient, SourceProtocolCreator>;

    template <typename Type>
    inline void register_source_protocol(const std::string &key)
    {
        auto &factory = SourceProtocolFactory::instance();
        factory.register_type(
            key,
            [](const Configuration &config,
               const DestinationRouterPtrVarient &destination_router,
               const SourceRouter &source_router)
            {
                return std::make_shared<Type>(config, destination_router, source_router);
            });
    }

    inline void register_all_source_protocols()
    {
        register_source_protocol<protocol_a::ProtocolA>("ProtocolA");
    }
}
