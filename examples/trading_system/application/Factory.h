#include <framework/factory/ValidatorFactory.h>
#include <framework/factory/DestinationProtocolFactory.h>
#include <framework/factory/DestinationRouterFactory.h>
#include <framework/factory/SourceProtocolFactory.h>
#include <framework/application_dependency/Validators.h>
#include <framework/application_dependency/DestinationProtocols.h>
#include <framework/application_dependency/DestinationRouters.h>
#include <framework/application_dependency/SourceSessions.h>

namespace hyper
{
    inline void register_all_validators()
    {
        framework::register_validator<ValidatorX>("ValidatorX");
    }

    inline void register_all_destination_protocols()
    {
        framework::register_destination_protocol<protocol_b::ProtocolB>("ProtocolB");
    }

    inline void register_all_destination_routers()
    {
        framework::register_destination_router<framework::DestinationRouterOneToOne>("OneToOne");
        framework::register_destination_router<framework::DestinationRouterRoundRobin>("RoundRobin");
    }

    inline void register_all_source_protocols()
    {
        framework::register_source_protocol<protocol_a::ProtocolA>("ProtocolA");
    }
}