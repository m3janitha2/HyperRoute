#include <framework/application/Application.h>
#include <framework/config/ConfigManager.h>
#include <framework/factory/ValidatorFactory.h>
#include <framework/factory/DestinationProtocolFactory.h>
#include <framework/factory/DestinationRouterFactory.h>
#include <framework/factory/SourceProtocolFactory.h>

#include <iostream>

namespace hyper::framework
{
    void Application::init(const std::string &config_file)
    {
        framework::ConfigManager::instance().init(config_file);
        source_router_ = std::make_unique<framework::SourceRouter>();
        load_validators();
        load_destination_protocol_sessions();
        load_source_protocol_sessions();
        std::cout << "Initialized application with config file: " << config_file << std::endl;
    }

    void Application::run()
    {
    }

    void Application::stop()
    {
    }

    ValidatorPtrVariant &Application::get_validator_by_id(std::size_t id)
    {
        if (auto it = validators_.find(id); it != validators_.end())
        {
            return it->second;
        }
        throw std::runtime_error("Validator not found for ID: " + std::to_string(id));
    }

    DestinationProtocolPtrVariant &Application::get_destination_protocol_session_by_id(std::size_t id)
    {
        if (auto it = destination_protocol_sessions_.find(id); it != destination_protocol_sessions_.end())
        {
            return it->second;
        }
        throw std::runtime_error("Destination protocol session not found for ID: " + std::to_string(id));
    }

    SourceProtocolPtrVariant &Application::get_source_protocol_session_by_id(std::size_t id)
    {
        if (auto it = source_protocol_sessions_.find(id); it != source_protocol_sessions_.end())
        {
            return it->second;
        }
        throw std::runtime_error("Source protocol session not found for ID: " + std::to_string(id));
    }

    void Application::load_validators()
    {
        for (auto &cfg_manager = framework::ConfigManager::instance();
             auto &config : cfg_manager.get_validators())
        {
            auto type = config.get<std::string>("type");
            auto id = config.get<std::size_t>("id");
            auto &factory = framework::ValidatorFactory::instance();
            auto validator = factory.create(type, config);
            validators_.emplace(id, validator);
        }
    }

    void Application::load_destination_protocol_sessions()
    {
        for (auto &cfg_manager = framework::ConfigManager::instance();
             auto &config : cfg_manager.get_destination_sessions())
        {
            auto id = config.get<std::size_t>("id");
            auto protocol_name = config.get<std::string>("protocol");
            auto validator_id = config.get<std::size_t>("validator_id");
            auto validator = get_validator_by_id(validator_id);
            auto &factory = framework::DestinationProtocolFactory::instance();
            auto protocol = factory.create(protocol_name,
                                           config, *source_router_.get(), validator);
            destination_protocol_sessions_.emplace(id, std::move(protocol));
        }
    }

    void Application::load_source_protocol_sessions()
    {
        for (const auto &config : framework::ConfigManager::instance().get_source_sessions())
        {
            const auto &router_config = config.get_child("router");
            const auto router_type = router_config.get<std::string>("type");
            auto &router_factory = framework::DestinationRouterFactory::instance();
            auto router = router_factory.create(router_type,
                                                router_config, destination_protocol_sessions_);

            auto &factory = framework::SourceProtocolFactory::instance();
            auto id = config.get<std::size_t>("id");
            auto protocol_name = config.get<std::string>("protocol");

            auto protocol = factory.create(protocol_name,
                                           config, router, *source_router_.get());
            source_protocol_sessions_.emplace(id, std::move(protocol));
        }
    }
}
