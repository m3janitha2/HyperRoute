#include <framework/config/ConfigManager.h>
#include <boost/property_tree/xml_parser.hpp>
#include <iostream>
#include "ConfigManager.h"

namespace hyper::framework
{
    ConfigManager &ConfigManager::instance()
    {
        static ConfigManager instance;
        return instance;
    }

    void ConfigManager::init(const std::string &config_file)
    {
        load_configuration(config_file);
    }

    void ConfigManager::load_configuration(const std::string &config_file)
    {
        pt::read_xml(config_file, root_config_);
        parse_child_config_by_name("configuration.source_sessions", source_sessions_, source_session_by_id_);
        parse_child_config_by_name("configuration.destination_sessions", destination_sessions_, destination_session_by_id_);
        parse_child_config_by_name("configuration.validators", validators_, validator_by_id_);
    }

    const std::vector<Configuration> &ConfigManager::get_source_sessions() const
    {
        return source_sessions_;
    }

    const std::vector<Configuration> &ConfigManager::get_destination_sessions() const
    {
        return destination_sessions_;
    }

    const std::vector<Configuration> &ConfigManager::get_validators() const
    {
        return validators_;
    }
    const Configuration &ConfigManager::get_source_session_by_id(std::size_t id) const
    {
        if (auto it = source_session_by_id_.find(id);
            it != source_session_by_id_.end())
        {
            return it->second;
        }
        else
            throw std::out_of_range("Source session ID not found");
    }

    const Configuration &ConfigManager::get_destination_session_by_id(std::size_t id) const
    {
        if (auto it = destination_session_by_id_.find(id);
            it != destination_session_by_id_.end())
        {
            return it->second;
        }
        else
            throw std::out_of_range("Destination session ID not found");
    }

    void ConfigManager::parse_child_config_by_name(const std::string &child_name,
                                                   std::vector<Configuration> &sessions,
                                                   std::unordered_map<std::size_t, Configuration> &session_by_id)
    {
        try
        {
            const auto &child_tree = root_config_.get_child(child_name);
            for (const auto &item : child_tree)
            {
                std::size_t id = 0;
                try
                {
                    id = item.second.get<std::size_t>("id");
                }
                catch (const pt::ptree_error &e)
                {
                    std::cerr << "Error extracting ID from session in " << child_name << ": " << e.what() << std::endl;
                    throw std::out_of_range("Missing or invalid 'id' in session in " + child_name);
                }

                Configuration session(item.second);
                sessions.emplace_back(session);
                session_by_id.emplace(id, std::move(session));
            }
        }
        catch (const pt::ptree_error &e)
        {
            std::cerr << "Error parsing child configuration '" << child_name << "': " << e.what() << std::endl;
            throw std::out_of_range("Error parsing session in configuration for: " + child_name);
        }
    }
}
