#include <iostream>
#include <stdexcept>
#include <string>
#include <random>
#include <memory>
#include <vector>
#include <framework/application/application.h>
#include <examples/trading_system/protocol/ProtocolA.h>
#include <examples/trading_system/protocol/ProtocolB.h>
#include <framework/simulator/Simulator.h>

using namespace hyper;

/** Random number generator for a specified range of type T. */
template <typename T>
class RandomGen
{
public:
    RandomGen(T start, T end, std::uint32_t seed = 1)
        : dist_(start, end), gen_(seed) {}

    T next() noexcept { return dist_(gen_); }

private:
    std::uniform_int_distribution<T> dist_;
    std::mt19937 gen_;
};

/** Parses command-line arguments */
std::size_t parse_arguments(int argc, char **argv)
{
    if (argc != 2)
    {
        throw std::invalid_argument("Usage: " + std::string(argv[0]) + " <positive_integer>");
    }

    try
    {
        int value = std::stoi(argv[1]);
        if (value <= 0)
        {
            throw std::invalid_argument("The argument must be a positive integer greater than 0.");
        }
        return static_cast<std::size_t>(value);
    }
    catch (const std::invalid_argument &)
    {
        throw std::invalid_argument("The argument is not a valid integer.");
    }
    catch (const std::out_of_range &)
    {
        throw std::out_of_range("The argument is out of range for an integer.");
    }
}

/** Initializes protocol sessions */
void initialize_protocol_sessions(framework::Application &application,
                                  std::shared_ptr<protocol_a::ProtocolA> &protocol_a,
                                  std::shared_ptr<protocol_b::ProtocolB> &protocol_b)
{
    // Retrieve protocol sessions by ID
    auto src_protocol_variant = application.get_source_protocol_session_by_id(2000);
    protocol_a = std::get<std::shared_ptr<protocol_a::ProtocolA>>(src_protocol_variant);

    auto dst_protocol_variant = application.get_destination_protocol_session_by_id(3000);
    protocol_b = std::get<std::shared_ptr<protocol_b::ProtocolB>>(dst_protocol_variant);
}

/** Simulate the order flow */
void simulate_order_flow(std::size_t number_of_messages,
                         framework::Simulator<protocol_a::ProtocolA> &sim_src,
                         framework::Simulator<protocol_b::ProtocolB> &sim_dst,
                         std::vector<protocol_a::schema::NewOrderSingle> &new_orders,
                         std::vector<protocol_b::schema::ExecutionReport> &execution_reports,
                         std::vector<protocol_a::schema::CancelReplaceRequest> &cancel_replaces)
{
    RandomGen<std::size_t> message_selector(0, number_of_messages - 1);

    for (std::size_t i = 0; i < number_of_messages; ++i)
    {
        auto msg_index = message_selector.next();

        // Simulate a NewOrderSingle
        auto &new_order = new_orders[msg_index];
        std::string_view new_order_data{reinterpret_cast<char *>(&new_order), new_order.header.size};

        // Client sends NewOrderSingle
        sim_src.send_request(new_order_data);

        // Venue receives NewOrderSingle
        if (!sim_dst.has_received_message())
        {
            std::cerr << "Error: Venue did not receive the NewOrderSingle." << std::endl;
            return;
        }
        auto &received_order = sim_dst.get_last_message<protocol_b::schema::NewOrderSingle>();
        std::cout << "Venue received: " << received_order << std::endl;

        // Simulate ExecutionReport (acknowledgment of order)
        auto &execution_report = execution_reports[msg_index];
		execution_report.c = received_order.c;
        std::string_view execution_report_data{reinterpret_cast<char *>(&execution_report), execution_report.header.size};

        // Client sends ExecutionReport
        sim_dst.send_request(execution_report_data);

        // Venue receives ExecutionReport
        if (!sim_src.has_received_message())
        {
            std::cerr << "Error: Venue did not receive the execution report." << std::endl;
            return;
        }
        auto &received_execution_report = sim_dst.get_last_message<protocol_a::schema::ExecutionReport>();
        std::cout << "Venue received execution report: " << received_execution_report << std::endl;

        // Simulate CancelReplaceRequest (if needed)
        auto &cancel_replace = cancel_replaces[msg_index];
		cancel_replace.orig_cl_ord_id = new_order.cl_ord_id;
        std::string_view cancel_replace_data{reinterpret_cast<char *>(&cancel_replace), cancel_replace.header.size};

        // Client sends CancelReplaceRequest
        sim_src.send_request(cancel_replace_data);

        // Venue receives CancelReplaceRequest
        if (!sim_dst.has_received_message())
        {
            std::cerr << "Error: Venue did not receive the cancel replace request." << std::endl;
            return;
        }
        auto &received_cancel_replace = sim_dst.get_last_message<protocol_a::schema::CancelReplaceRequest>();
        std::cout << "Venue received cancel replace request: " << received_cancel_replace << std::endl;
    }
}

int main(int argc, char **argv)
{
    try
    {
        // Get the number of messages to process from command line argument
        const auto number_of_messages = parse_arguments(argc, argv);
        std::cout << "Number of messages to process: " << number_of_messages << std::endl;

        // Initialize the application
        auto &application = framework::Application::instance();
        application.init("/mnt/d/linux/m3janitha2/github/HyperRoute/examples/trading_system/config/config.xml");

        // Initialize protocol sessions
        std::shared_ptr<protocol_a::ProtocolA> protocol_a;
        std::shared_ptr<protocol_b::ProtocolB> protocol_b;
        initialize_protocol_sessions(application, protocol_a, protocol_b);

        // Output source and destination protocol names
        std::cout << "Source: " << protocol_a->name() << std::endl
                  << "Destination: " << protocol_b->name() << std::endl;

        // Simulators for source and destination protocols
        framework::Simulator<protocol_a::ProtocolA> sim_src(*protocol_a);
        framework::Simulator<protocol_b::ProtocolB> sim_dst(*protocol_b);

        // Create vectors to hold protocol schema messages
        std::vector<protocol_a::schema::NewOrderSingle> new_orders;
        std::vector<protocol_b::schema::ExecutionReport> execution_reports;
        std::vector<protocol_a::schema::CancelReplaceRequest> cancel_replaces;

        // Random generators for different types
        RandomGen<std::uint32_t> uint32_generator(1, 1000);
        RandomGen<std::uint64_t> uint64_generator(1, 1000);
        RandomGen<int> int_generator(1, 1000);

        // Generate messages
        for (std::size_t i = 0; i < number_of_messages; ++i)
        {
            new_orders.emplace_back(protocol_a::schema::NewOrderSingle{
                .a = uint32_generator.next(),
                .b = uint32_generator.next(),
                .cl_ord_id = uint64_generator.next()});

            execution_reports.emplace_back(protocol_b::schema::ExecutionReport{
                .a = int_generator.next(),
                .b = int_generator.next(),
                .c = int_generator.next()});

            cancel_replaces.emplace_back(protocol_a::schema::CancelReplaceRequest{
                .a = uint32_generator.next(),
                .b = uint32_generator.next(),
                .cl_ord_id = uint64_generator.next(),
                .orig_cl_ord_id = uint64_generator.next()});
        }

        // Optionally print the number of generated messages for verification
        std::cout << "Generated " << number_of_messages << " messages for NewOrderSingle, ExecutionReport, and CancelReplaceRequest." << std::endl;

        // Simulate order flow
        simulate_order_flow(number_of_messages, sim_src, sim_dst, new_orders, execution_reports, cancel_replaces);

        return EXIT_SUCCESS;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
}
