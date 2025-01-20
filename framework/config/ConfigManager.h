
#pragma once

#include <framework/config/Configuration.h>
#include <boost/property_tree/ini_parser.hpp>
#include <unordered_map>
#include <vector>
#include <string>
#include <iostream>

namespace hyper::framework
{
    class ConfigManager
    {
    public:
        static ConfigManager &instance();
        void init(const std::string &config_file);

        const std::vector<Configuration> &get_source_sessions() const;
        const std::vector<Configuration> &get_destination_sessions() const;
        const std::vector<Configuration> &get_validators() const;

        const Configuration &get_source_session_by_id(std::size_t id) const;
        const Configuration &get_destination_session_by_id(std::size_t id) const;

    private:
        ConfigManager() = default;

        void load_configuration(const std::string &config_file);
        void parse_child_config_by_name(const std::string &child_name,
                                        std::vector<Configuration> &sessions,
                                        std::unordered_map<std::size_t, Configuration> &session_by_id);

        Configuration root_config_;
        std::vector<Configuration> source_sessions_;
        std::vector<Configuration> destination_sessions_;
        std::vector<Configuration> validators_;
        std::unordered_map<std::size_t, Configuration> source_session_by_id_;
        std::unordered_map<std::size_t, Configuration> destination_session_by_id_;
        std::unordered_map<std::size_t, Configuration> validator_by_id_;
    };
}
