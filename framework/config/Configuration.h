#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <string>

namespace hyper::framework
{
    namespace pt = boost::property_tree;
    using Config = pt::ptree;

    class Configuration
    {
    public:
        explicit Configuration(const std::string &filename);

        Configuration(const Configuration &) = delete;
        Configuration &operator=(const Configuration &) = delete;

        Config &source_sesssion_cfg_by_name(const std::string &session_name);
        Config &destination_session_cfg_by_name(const std::string &session_name);

    private:
        void load(const std::string &filename, Config &config_) const;
        Config &cfg_by_name(const std::string &name, const std::string &config_name);
        const std::string &filename_;
        Config config_;
    };
}