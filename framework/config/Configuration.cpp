#include <framework/config/Configuration.h>
#include <iostream>
#include "Configuration.h"

namespace hyper::framework
{
    Configuration::Configuration(const std::string &filename) : filename_(filename)
    {
        load(filename_, config_);
        std::cout << config_.get<std::string>("configuration.filename") << std::endl;
    }

    void Configuration::load(const std::string &filename, Config &config) const
    {
        pt::read_xml(filename, config);
    }

    Config &Configuration::cfg_by_name(const std::string &name, const std::string &config_name)
    {
        for (auto &&subtree : config_.get_child(config_name))
        {
            if (subtree.second.get<std::string>("name") == name)
                return subtree.second;
        }
        std::string error = "invalid" + config_name;
        throw std::runtime_error(error);
    }

    Config &Configuration::source_sesssion_cfg_by_name(const std::string &session_name)
    {
        return cfg_by_name(session_name, "configuration.source_sessions");
    }

    Config &Configuration::destination_session_cfg_by_name(const std::string &session_name)
    {
        return cfg_by_name(session_name, "configuration.destination_sessions");
    }
}