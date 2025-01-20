#pragma once

#include <framework/config/ConfigManager.h>
#include <framework/application_dependency/DestinationProtocols.h>
#include <framework/application_dependency/SourceProtocolDeclarations.h>
#include <memory>
#include <string>
#include <unordered_map>
#include <stdexcept>

namespace hyper::framework
{
    class Application
    {
    public:
        Application(const Application &) = delete;
        Application &operator=(const Application &) = delete;

        static Application &instance()
        {
            static Application app;
            return app;
        }

        void init(const std::string &config_file);
        void run();
        void stop();

        ValidatorPtrVarient &get_validator_by_id(std::size_t id);
        DestinationProtocolPtrVarient &get_destination_protocol_session_by_id(std::size_t id);
        SourceProtocolPtrVarient &get_source_protocol_session_by_id(std::size_t id);

    private:
        Application() = default;

        void load_validators();
        void load_destination_protocol_sessions();
        void load_source_protocol_sessions();

        std::unique_ptr<framework::SourceRouter> source_router_;
        std::unordered_map<std::size_t, ValidatorPtrVarient> validators_{};
        std::unordered_map<std::size_t, DestinationProtocolPtrVarient> destination_protocol_sessions_{};
        std::unordered_map<std::size_t, SourceProtocolPtrVarient> source_protocol_sessions_{};
    };
}
