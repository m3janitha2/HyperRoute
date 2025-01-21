#include <iostream>
#include <stdexcept>
#include <string>
#include <random>
#include <memory>
#include <vector>
#include <framework/application/Application.h>
#include <framework/simulator/Simulator.h>
#include <examples/trading_system/protocol/ProtocolA.h>
#include <examples/trading_system/protocol/ProtocolB.h>
#include <examples/trading_system/application/TestMessageStore.h>

using namespace hyper;

/** Parses command-line arguments to retrieve config file path and number of messages */
std::pair<std::string, std::size_t> parse_arguments(int argc, char **argv)
{
    if (argc != 3)
    {
        throw std::invalid_argument("Usage: " + std::string(argv[0]) + " <path_to_config_file> <number_of_messages>");
    }

    std::string config_file_path = argv[1];
    if (config_file_path.empty())
    {
        config_file_path = "/mnt/d/linux/m3janitha2/github/HyperRoute/examples/trading_system/config/config.xml";
        std::cerr << "Warning: Config file path was empty. Using default path: " << config_file_path << std::endl;
    }

    try
    {
        int value = std::stoi(argv[2]);
        if (value <= 0)
        {
            throw std::invalid_argument("The number of messages must be a positive integer greater than 0.");
        }
        return {config_file_path, static_cast<std::size_t>(value)};
    }
    catch (const std::invalid_argument &)
    {
        throw std::invalid_argument("The second argument (number of messages) is not a valid integer.");
    }
    catch (const std::out_of_range &)
    {
        throw std::out_of_range("The number of messages is out of range for an integer.");
    }
}

/** Initializes protocol sessions */
auto initialize_protocol_sessions(framework::Application &application)
{
	// Retrieve protocol sessions by ID (from config)
	auto src_protocol_variant = application.get_source_protocol_session_by_id(2000);
	auto protocol_a = std::get<std::shared_ptr<protocol_a::ProtocolA>>(src_protocol_variant);

	auto dst_protocol_variant = application.get_destination_protocol_session_by_id(3000);
	auto protocol_b = std::get<std::shared_ptr<protocol_b::ProtocolB>>(dst_protocol_variant);

	if (!protocol_a || !protocol_b)
	{
		throw std::runtime_error("Failed to initialize protocol sessions.");
	}

	return std::pair{protocol_a, protocol_b};
}

/** Simulate the order flow */
int simulate_order_flow(std::size_t number_of_messages,
						framework::Simulator<protocol_a::ProtocolA> &sim_src,
						framework::Simulator<protocol_b::ProtocolB> &sim_dst,
						TestMessageStore &msg_store)
{
	static std::uint64_t cl_ord_id{1000};

	for (std::size_t i = 0; i < number_of_messages; ++i)
	{
		// Client sends NewOrderSingle
		sim_src.send_request(msg_store.get_new_order(cl_ord_id++));

		// Venue receives NewOrderSingle
		if (!sim_dst.has_received_message())
		{
			std::cerr << "Error: Venue did not receive the NewOrderSingle." << std::endl;
			return EXIT_FAILURE;
		}
		auto &received_order = sim_dst.get_last_message<protocol_b::schema::NewOrderSingle>();
		std::cout << "Venue received: " << received_order << std::endl;

		// Venue sends ExecutionReport
		sim_dst.send_request(msg_store.get_execution_report(received_order.c));

		// Client receives ExecutionReport
		if (!sim_src.has_received_message())
		{
			std::cerr << "Error: Client did not receive the ExecutionReport." << std::endl;
			return EXIT_FAILURE;
		}
		auto &received_execution_report = sim_src.get_last_message<protocol_a::schema::ExecutionReport>();
		std::cout << "Client received: " << received_execution_report << std::endl;

		// Client sends CancelReplaceRequest
		sim_src.send_request(msg_store.get_cancel_replace(received_execution_report.cl_ord_id));

		// Venue receives CancelReplaceRequest
		if (!sim_dst.has_received_message())
		{
			std::cerr << "Error: Venue did not receive the CancelReplaceRequest." << std::endl;
			return EXIT_FAILURE;
		}
		auto &received_cancel_replace = sim_dst.get_last_message<protocol_b::schema::CancelReplaceRequest>();
		std::cout << "Venue received: " << received_cancel_replace << std::endl;
	}
	return EXIT_SUCCESS;
}

int main(int argc, char **argv)
{
	try
	{
        // Parse command-line arguments
        auto [config_file_path, number_of_messages] = parse_arguments(argc, argv);
		std::cout << "Number of messages to process: " << number_of_messages << std::endl;

		// Initialize the application
		auto &application = framework::Application::instance();
		application.init(config_file_path);

		// Initialize protocol sessions
		auto [protocol_a, protocol_b] = initialize_protocol_sessions(application);

		// Simulators for source and destination
		framework::Simulator<protocol_a::ProtocolA> sim_src(*protocol_a);
		framework::Simulator<protocol_b::ProtocolB> sim_dst(*protocol_b);

		// Message store to generate test messages
		TestMessageStore msg_store(number_of_messages);

		// Simulate the order flow
		return simulate_order_flow(number_of_messages, sim_src, sim_dst, msg_store);
	}
	catch (const std::exception &e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
		return EXIT_FAILURE;
	}
}