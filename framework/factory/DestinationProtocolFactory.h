
#pragma once

#include <framework/factory/Factory.h>
#include <framework/application_dependency/DestinationProtocols.h>
#include <framework/application_dependency/Validators.h>
#include <framework/config/Configuration.h>

namespace hyper::framework
{
    class SourceRouter;

    using DestinationProtocolCreator =
        std::function<DestinationProtocolPtrVarient(const Configuration &config,
                                                    SourceRouter &source_router,
                                                    ValidatorPtrVarient &validator)>;

    using DestinationProtocolFactory = Factory<DestinationProtocolCreator>;

    template <typename Type>
    inline void register_destination_protocol(const std::string &key)
    {
        auto &factory = DestinationProtocolFactory::instance();
        factory.register_type(
            key,
            [](const Configuration &config,
               SourceRouter &source_router,
               ValidatorPtrVarient &validator)
            {
                return std::make_shared<Type>(config, source_router, validator);
            });
    }

    inline void register_all_destination_protocols()
    {
        register_destination_protocol<protocol_b::ProtocolB>("ProtocolB");
    }
}
