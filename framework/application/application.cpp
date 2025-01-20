#include <framework/application/Application.h>
#include <framework/factory/ValidatorFactory.h>
#include <framework/factory/DestinationProtocolFactory.h>
#include <framework/factory/DestinationRouterFactory.h>
#include <framework/factory/SourceProtocolFactory.h>
#include <framework/application_dependency/SourceProtocols.h>
#include <iostream>

namespace hyper::framework
{
    void Application::init(const std::string &config_file)
    {
        source_router_ = std::make_unique<framework::SourceRouter>();
        load_validators();
        load_destination_protocol_sessions();
        load_source_protocol_sessions();
        std::cout << "Initialized application with config file: " << config_file << std::endl;
    }

    void Application::run()
    {
        // if (source_router_)
        // {
        //     source_router_->start();
        //     std::cout << "Application is running..." << std::endl;
        // }
        // else
        // {
        //     throw std::runtime_error("Source router is not initialized.");
        // }
    }

    void Application::stop()
    {
        // if (source_router_)
        // {
        //     source_router_->stop();
        //     std::cout << "Application has stopped." << std::endl;
        // }
    }

    ValidatorPtrVarient &Application::get_validator_by_id(std::size_t id)
    {
        if (auto it = validators_.find(id); it != validators_.end())
        {
            return it->second;
        }
        throw std::runtime_error("Validator not found for ID: " + std::to_string(id));
    }

    DestinationProtocolPtrVarient &Application::get_destination_protocol_session_by_id(std::size_t id)
    {
        if (auto it = destination_protocol_sessions_.find(id); it != destination_protocol_sessions_.end())
        {
            return it->second;
        }
        throw std::runtime_error("Destination protocol session not found for ID: " + std::to_string(id));
    }

    // SourceProtocolPtrVarient &Application::get_source_protocol_session_by_id(std::size_t id)
    // {
    //     if (auto it = source_protocol_sessions_.find(id); it != source_protocol_sessions_.end())
    //     {
    //         return it->second;
    //     }
    //     throw std::runtime_error("Source protocol session not found for ID: " + std::to_string(id));
    // }

    void Application::load_validators()
    {
        framework::register_all_validators();

        for (auto &cfg_manager = framework::ConfigManager::instance();
             auto &config : cfg_manager.get_validators())
        {
            auto type = config.get<std::string>("type");
            auto id = config.get<std::size_t>("id");
            auto &factory = framework::ValidatorFactory::instance();
            auto validator = factory.create(type, config);
            validators_.emplace(id, std::move(validator));
        }
    }

    void Application::load_destination_protocol_sessions()
    {
        framework::register_all_destination_protocols();

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
        framework::register_all_destination_routers();
        framework::register_all_source_protocols();

        for (auto &cfg_manager = framework::ConfigManager::instance();
             auto &config : cfg_manager.get_destination_sessions())
        {
            auto &router_config = config.get_child("router");
            auto router_type = config.get<std::string>("type");
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
