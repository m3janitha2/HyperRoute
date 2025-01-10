
#include <framework/config/Configuration.h>
#include <framework/router/SourceRouter.h>
#include <framework/application_dependency/DestinationRouters.h>
#include <framework/application_dependency/DestinationSessions.h>
#include <framework/application_dependency/Validators.h>
#include <example/trading_system/protocol/ProtocolA.h>
#include <example/trading_system/protocol/ProtocolB.h>
#include <string.h>
#include <random>
#include <vector>
#include <string>

using namespace hyper;
/* Temporary implementation until the config parser is implemented */
/* A sample config file is located at application/config/config.xml */
struct SubSystem
{
	SubSystem()
	{
		/* todox: */
		// Configuration cfg{"application/config/config.xml"};
		// auto x = cfg.source_sesssion_cfg_by_name("SourceA");
	}

	/* These will be created by factories within the constructor of the owner */
	framework::SourceRouter source_router{};
	ValidatorX validator{};
	ValidatorPtrVarient validator_varient{&validator};
	protocol_b::ProtocolB destination_protocol{source_router, validator_varient};
	DestinationSessionPtrVarient destination_session_varient{&destination_protocol.session()};
	std::vector<DestinationSessionPtrVarient *> destination_sessions{const_cast<DestinationSessionPtrVarient *>(&destination_session_varient)};
	framework::DestinationRouterOneToOne dest_router_one_to_one{destination_session_varient};
	framework::DestinationRouterRoundRobin dest_router_round_robin{destination_sessions};
	DestinationRouterPtrVarient dest_router_variant{&dest_router_round_robin};
	protocol_a::ProtocolA source_protocol{dest_router_variant, source_router};
};

/* This is a basic simulator designed to simulate a client (order source) and a venue (exchange) */
struct Simulator
{
	Simulator()
	{
		subsystem_.destination_protocol.transport().set_receive_data_cb_for_test([this](std::string_view data)
																				 { on_data_from_destination_session(data); });
		subsystem_.source_protocol.transport().set_receive_data_cb_for_test([this](std::string_view data)
																			{ on_data_from_source_session(data); });
	}

	void on_data_from_source_session(std::string_view data)
	{
		// std::cout << "client recevied message" << std::endl;
		set_msg_received_by_source(true);
		no_of_msgs_received_by_src_++;
		set_last_recevied_msg(data);
	}

	void on_data_from_destination_session(std::string_view data)
	{
		// std::cout << "venue recevied message" << std::endl;
		set_msg_received_by_destination(true);
		no_of_msgs_received_by_dest_++;
		set_last_recevied_msg(data);
	}

	void send_request_to_source_session(std::string_view data)
	{
		/* Reset the destination before the request is sent from the source */
		set_msg_received_by_destination(false);
		no_of_msgs_sent_by_src_++;
		subsystem_.source_protocol.transport().on_data(data);
	}

	void send_response_to_destination_session(std::string_view data)
	{
		/* Reset the source before the response is sent from the destination */
		set_msg_received_by_source(false);
		no_of_msgs_sent_by_dest_++;
		subsystem_.destination_protocol.transport().on_data(data);
	}

	void set_last_recevied_msg(std::string_view data)
	{
		memcpy(buffer_, data.data(), data.length());
	}

	template <typename Msg>
	Msg &get_last_msg()
	{
		return *reinterpret_cast<Msg *>(buffer_);
	}

	[[nodiscard]] constexpr bool is_msg_received_by_destination() const noexcept { return msg_received_by_destination_; }
	constexpr void set_msg_received_by_destination(bool is_received) noexcept { msg_received_by_destination_ = is_received; }

	[[nodiscard]] constexpr bool is_msg_received_by_source() const noexcept { return msg_received_by_destination_; }
	constexpr void set_msg_received_by_source(bool is_received) noexcept { msg_received_by_destination_ = is_received; }

	void print_stat()
	{
		std::cout << "no_of_msgs_sent_by_src:" << no_of_msgs_sent_by_src_ << std::endl
				  << "no_of_msgs_received_by_dest:" << no_of_msgs_received_by_dest_ << std::endl
				  << "no_of_msgs_sent_by_dest:" << no_of_msgs_sent_by_dest_ << std::endl
				  << "no_of_msgs_received_by_src:" << no_of_msgs_received_by_src_ << std::endl;
	}

	SubSystem subsystem_{};
	bool msg_received_by_destination_{false};
	bool msg_received_by_source_{false};
	std::size_t no_of_msgs_sent_by_src_{0};
	std::size_t no_of_msgs_received_by_src_{0};
	std::size_t no_of_msgs_sent_by_dest_{0};
	std::size_t no_of_msgs_received_by_dest_{0};
	char buffer_[1024]{};
};

template <typename T>
struct RandomGen
{
	RandomGen(T start, T end, std::mt19937 gen)
		: dist(start, end), gen(gen) {}

	std::uniform_int_distribution<T> dist;
	std::mt19937 gen;
	T next() { return dist(gen); }
};

int parse_arguments(int argc, char **argv)
try
{
	if (argc != 2)
	{
		std::cerr << "Usage: " << argv[0] << " <integer>\n";
		return -1;
	}

	return std::stoi(argv[1]);
}
catch (const std::invalid_argument &e)
{
	std::cerr << "Error: The argument is not a valid integer.\n";
	return -1;
}

int main(int argc, char **argv)
{
	const auto number_of_messages = parse_arguments(argc, argv);

	Simulator sim{};
	std::vector<protocol_a::schema::NewOrderSingle> new_orders{};
	std::vector<protocol_b::schema::ExecutionReport> new_acks{};
	std::vector<protocol_a::schema::CancelReplaceRequest> cancel_replaces{};

	std::random_device rd;
	std::mt19937 gen(rd());
	RandomGen<std::uint32_t> g_uint32(1, 1000, gen);
	RandomGen<std::uint64_t> g_uint64(1, 1000, gen);
	RandomGen<int> g_int(1, 1000, gen);

	for (auto i{0}; i < number_of_messages; i++)
	{
		new_orders.emplace_back(protocol_a::schema::NewOrderSingle{.a = g_uint32.next(),
																		  .b = g_uint32.next(),
																		  .cl_ord_id = g_uint64.next()});
		new_acks.emplace_back(protocol_b::schema::ExecutionReport{.a = g_int.next(),
																		 .b = g_int.next(),
																		 .c = g_int.next()});
		cancel_replaces.emplace_back(protocol_a::schema::CancelReplaceRequest{.a = g_uint32.next(),
																					 .b = g_uint32.next(),
																					 .cl_ord_id = g_uint64.next(),
																					 .orig_cl_ord_id = g_uint64.next()});
	}

	RandomGen<std::size_t> g_size_t(0, number_of_messages - 1, gen);

	for (auto i{0}; i < number_of_messages; i++)
	{
		auto msg_index = g_size_t.next();

		/* Simulate an order chain */
		auto &new_order = new_orders[msg_index];
		std::string_view new_order_data{reinterpret_cast<char *>(&new_order), new_order.header.size};

		/* Client sends NewOrderSingle */
		sim.send_request_to_source_session(new_order_data);

		/* Venue received NewOrderSingle */
		if (!sim.is_msg_received_by_destination()) [[unlikely]]
			return -1;
		auto &dest_new = sim.get_last_msg<protocol_b::schema::NewOrderSingle>();
		std::cout << "venue received: " << dest_new << std::endl;

		/* Venue sends ExecutionReport */
		auto &new_ack = new_acks[msg_index];
		new_ack.c = dest_new.c;
		std::string_view new_ack_data{reinterpret_cast<char *>(&new_ack), new_ack.header.size};
		sim.send_response_to_destination_session(new_ack_data);

		/* Client received ExecutionReport */
		if (!sim.is_msg_received_by_source()) [[unlikely]]
			return -1;
		auto &src_new_ack = sim.get_last_msg<protocol_a::schema::ExecutionReport>();
		std::cout << "client received: " << src_new_ack << std::endl;

		/* Client sends CancelReplaceRequest */
		auto &amend = cancel_replaces[msg_index];
		amend.orig_cl_ord_id = new_order.cl_ord_id;
		std::string_view amend_data{reinterpret_cast<char *>(&amend), amend.header.size};
		sim.send_request_to_source_session(amend_data);

		/* Venue received CancelReplaceRequest */
		if (!sim.is_msg_received_by_source()) [[unlikely]]
			return -1;
		auto &dst_amend = sim.get_last_msg<protocol_b::schema::CancelReplaceRequest>();
		std::cout << "venue received: " << dst_amend << std::endl;
	}

	sim.print_stat();
	return 0;
}