
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

    using DestinationProtocolFactory =
        Factory<DestinationProtocolPtrVarient, DestinationProtocolCreator>;

    inline void register_all_protocols()
    {
        auto &factory = DestinationProtocolFactory::instance();

        factory.register_type("ProtocolB",
                              [](const Configuration &config,
                                 SourceRouter &source_router,
                                 ValidatorPtrVarient &validator)
                              {
                                  return std::make_shared<protocol_b::ProtocolB>(config,
                                                                                 source_router,
                                                                                 validator);
                              });
    }
}
