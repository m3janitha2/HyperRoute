#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <string>
#include <optional>
#include <string_view>

namespace pt = boost::property_tree;

class Configuration
{
public:
    explicit Configuration(const std::string &filename);

    Configuration(const Configuration &) = delete;
    Configuration &operator=(const Configuration &) = delete;

    pt::ptree& get_client_ses_cfg_by_name(const std::string& session_name);
    pt::ptree& get_venue_ses_cfg_by_name(const std::string& session_name);

private:
    void load(const std::string &filename, pt::ptree& config_) const;
    const std::string &filename_;
    pt::ptree config_;
};

inline Configuration::Configuration(const std::string &filename):filename_(filename)
{
    load(filename_, config_);
    std::cout << config_.get<std::string>("configuration.filename")  << std::endl;
}

inline pt::ptree &Configuration::get_client_ses_cfg_by_name(const std::string &session_name)
{
    for(auto&& subtree : config_.get_child("configuration.client_sessions"))
    {
        if(subtree.second.get<std::string>("name") == session_name)
            return subtree.second;
    }
    throw std::runtime_error("invalid client session name");
}

inline pt::ptree &Configuration::get_venue_ses_cfg_by_name(const std::string &session_name)
{
    for(auto&& subtree : config_.get_child("configuration.venue_sessions"))
    {
        if(subtree.second.get<std::string>("name") == session_name)
            return subtree.second;
    }
    throw std::runtime_error("invalid venue session name");
}

inline void Configuration::load(const std::string &filename, pt::ptree &config) const
{
    pt::read_xml(filename,config);
}