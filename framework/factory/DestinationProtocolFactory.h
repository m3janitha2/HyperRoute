
#pragma once

#include <framework/factory/Factory.h>
#include <framework/config/Configuration.h>
#include <framework/application_dependency/ValidatorDeclarations.h>
#include <framework/application_dependency/DestinationProtocolDeclarations.h>

namespace hyper::framework
{
    class SourceRouter;

    using DestinationProtocolCreator =
        std::function<DestinationProtocolPtrVariant(const Configuration &config,
                                                    SourceRouter &source_router,
                                                    const ValidatorPtrVariant &validator)>;

    using DestinationProtocolFactory = Factory<DestinationProtocolCreator>;

    template <typename Type>
    inline void register_destination_protocol(const std::string &key)
    {
        auto &factory = DestinationProtocolFactory::instance();
        factory.register_type(
            key,
            [](const Configuration &config,
               SourceRouter &source_router,
               const ValidatorPtrVariant &validator)
            {
                return std::make_shared<Type>(config, source_router, validator);
            });
    }
}
