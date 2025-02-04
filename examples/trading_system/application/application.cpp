#include <iostream>
#include <stdexcept>
#include <string>
#include <random>
#include <memory>
#include <vector>
#include <framework/application/Application.h>
#include <examples/trading_system/protocol/ProtocolA.h>
#include <examples/trading_system/protocol/ProtocolB.h>
#include <examples/trading_system/application/TestMessageStore.h>
#include <examples/trading_system/application/Factory.h>

using namespace hyper;

/** Parses command-line arguments to retrieve config file path and number of messages */
std::string parse_arguments(int argc, char **argv)
{
	if (argc != 2)
	{
		throw std::invalid_argument("Usage: " + std::string(argv[0]) + " <path_to_config_file>");
	}

	std::string config_file_path = argv[1];
	if (config_file_path.empty())
	{
		config_file_path = "/mnt/d/linux/m3janitha2/github/HyperRoute/examples/trading_system/config/config.xml";
		std::cerr << "Warning: Config file path was empty. Using default path: " << config_file_path << std::endl;
	}

	return config_file_path;
}

int main(int argc, char **argv)
{
	try
	{
		// Parse command-line arguments
		auto config_file_path = parse_arguments(argc, argv);
		std::cout << "config_file_path: " << config_file_path << std::endl;

		// Register all types
		register_all_validators();
		register_all_destination_protocols();
		register_all_destination_routers();
		register_all_source_protocols();

		// Initialize the application
		auto &application = framework::Application::instance();
		application.init(config_file_path);
		application.run();
		application.stop();
	}
	catch (const std::exception &e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
		return EXIT_FAILURE;
	}
}