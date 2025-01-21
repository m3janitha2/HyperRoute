
#include <string.h>
#include <random>
#include <vector>
#include <string>
#include <framework/application/application.h>
#include <framework/simulator/Simulator.h>

using namespace hyper;

template <typename T>
struct RandomGen
{
	RandomGen(T start, T end, std::uint32_t seed = 1)
		: dist(start, end), gen(seed) {}

	std::uniform_int_distribution<T> dist;
	std::mt19937 gen;
	T next() { return dist(gen); }
};

std::size_t parse_arguments(int argc, char **argv)
{
    if (argc != 2)
        throw std::invalid_argument("Usage: " + std::string(argv[0]) + " <positive_integer>");

    try
    {
        int value = std::stoi(argv[1]);
        if (value <= 0)
            throw std::invalid_argument("Error: The argument must be a positive integer greater than 0.");

        return static_cast<std::size_t>(value);
    }
    catch (const std::invalid_argument &)
    {
        throw std::invalid_argument("Error: The argument is not a valid integer.");
    }
    catch (const std::out_of_range &)
    {
        throw std::out_of_range("Error: The argument is out of range for an integer.");
    }
}


int main(int argc, char **argv)
{
	const auto number_of_messages = parse_arguments(argc, argv);

	auto& application = framework::Application::instance();
	application.init("/mnt/d/linux/m3janitha2/github/HyperRoute/examples/trading_system/config/config.xml");

	auto src_pt = application.get_source_protocol_session_by_id(100);
	auto protocol_a = std::get<std::shared_ptr<protocol_a::ProtocolA>>(src_pt);

	auto dst_pt = application.get_source_protocol_session_by_id(200);
	auto protocol_a = std::get<std::shared_ptr<protocol_b::ProtocolB>>(source_pt);

	// Simulator sim{};
	// std::vector<protocol_a::schema::NewOrderSingle> new_orders{};
	// std::vector<protocol_b::schema::ExecutionReport> new_acks{};
	// std::vector<protocol_a::schema::CancelReplaceRequest> cancel_replaces{};

	// RandomGen<std::uint32_t> g_uint32(1, 1000);
	// RandomGen<std::uint64_t> g_uint64(1, 1000);
	// RandomGen<int> g_int(1, 1000);

	// for (auto i{0}; i < number_of_messages; i++)
	// {
	// 	new_orders.emplace_back(protocol_a::schema::NewOrderSingle{.a = g_uint32.next(),
	// 															   .b = g_uint32.next(),
	// 															   .cl_ord_id = g_uint64.next()});
	// 	new_acks.emplace_back(protocol_b::schema::ExecutionReport{.a = g_int.next(),
	// 															  .b = g_int.next(),
	// 															  .c = g_int.next()});
	// 	cancel_replaces.emplace_back(protocol_a::schema::CancelReplaceRequest{.a = g_uint32.next(),
	// 																		  .b = g_uint32.next(),
	// 																		  .cl_ord_id = g_uint64.next(),
	// 																		  .orig_cl_ord_id = g_uint64.next()});
	// }

	// RandomGen<std::size_t> g_size_t(0, number_of_messages - 1);

	// for (auto i{0}; i < number_of_messages; i++)
	// {
	// 	auto msg_index = g_size_t.next();

	// 	/* Simulate an order chain */
	// 	auto &new_order = new_orders[msg_index];
	// 	std::string_view new_order_data{reinterpret_cast<char *>(&new_order), new_order.header.size};

	// 	/* Client sends NewOrderSingle */
	// 	sim.send_request_to_source_session(new_order_data);

	// 	/* Venue received NewOrderSingle */
	// 	if (!sim.is_msg_received_by_destination()) [[unlikely]]
	// 		return -1;
	// 	auto &dest_new = sim.get_last_msg<protocol_b::schema::NewOrderSingle>();
	// 	std::cout << "venue received: " << dest_new << std::endl;

	// 	/* Venue sends ExecutionReport */
	// 	auto &new_ack = new_acks[msg_index];
	// 	new_ack.c = dest_new.c;
	// 	std::string_view new_ack_data{reinterpret_cast<char *>(&new_ack), new_ack.header.size};
	// 	sim.send_response_to_destination_session(new_ack_data);

	// 	/* Client received ExecutionReport */
	// 	if (!sim.is_msg_received_by_source()) [[unlikely]]
	// 		return -1;
	// 	auto &src_new_ack = sim.get_last_msg<protocol_a::schema::ExecutionReport>();
	// 	std::cout << "client received: " << src_new_ack << std::endl;

	// 	/* Client sends CancelReplaceRequest */
	// 	auto &amend = cancel_replaces[msg_index];
	// 	amend.orig_cl_ord_id = new_order.cl_ord_id;
	// 	std::string_view amend_data{reinterpret_cast<char *>(&amend), amend.header.size};
	// 	sim.send_request_to_source_session(amend_data);

	// 	/* Venue received CancelReplaceRequest */
	// 	if (!sim.is_msg_received_by_source()) [[unlikely]]
	// 		return -1;
	// 	auto &dst_amend = sim.get_last_msg<protocol_b::schema::CancelReplaceRequest>();
	// 	std::cout << "venue received: " << dst_amend << std::endl;
	// }

	// sim.print_stat();
	return 0;
}