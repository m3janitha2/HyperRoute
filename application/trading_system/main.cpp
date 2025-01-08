
#include <framework/router/DestinationRouters.h>
#include <framework/router/SourceRouter.h>
#include <framework/config/Configuration.h>
#include <application/session/DestinationSessions.h>
#include <application/session/ProtocolA.h>
#include <application/session/ProtocolB.h>
#include <application/session/DestinationSessionProtocolB.h>
#include <string.h>

/* Temporary implementation until the config parser is implemented. */
/* A sample config file is located at application/config/config.xml */
struct SubSystem
{
	SubSystem()
	{
		/* todox: */
		// Configuration cfg{"application/config/config.xml"};
		// auto x = cfg.get_source_ses_cfg_by_name("SourceA");
	}

	/* These will be created by factories within the constructor of the owner */
	hyper::framework::SourceRouter source_router{};
	hyper::protocol_b::ProtocolB destination_protocol{source_router};
	const hyper::DestinationSessionPtrVarient destination_session_varient{&destination_protocol.session()};
	hyper::framework::DestinationRouterOneToOne forwar_router{const_cast<hyper::DestinationSessionPtrVarient &>(destination_session_varient)};
	hyper::DestinationRouterPtrVarient forward_router_variant{&forwar_router};
	hyper::protocol_a::ProtocolA source_protocol{forward_router_variant, source_router};
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

	void on_data_from_destination_session(std::string_view data)
	{
		// std::cout << "venue recevied message" << std::endl;
		set_msg_received_by_destination(true);
		set_last_recevied_msg(data);
	}
	void on_data_from_source_session(std::string_view data)
	{
		// std::cout << "client recevied message" << std::endl;
		set_msg_received_by_source(true);
		set_last_recevied_msg(data);
	}

	void send_request_to_source_session(std::string_view data)
	{
		/* Reset the destination before the request is sent from the source */
		set_msg_received_by_destination(false);
		subsystem_.source_protocol.transport().on_data(data);
	}

	void send_response_to_destination_session(std::string_view data)
	{
		/* Reset the source before the response is sent from the destination */
		set_msg_received_by_source(false);
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

	SubSystem subsystem_{};
	bool msg_received_by_destination_{false};
	bool msg_received_by_source_{false};
	char buffer_[1024]{};
};

int main(int argc, char **argv)
{
	Simulator sim{};

	for (auto i{0}; i < 1000; i++)
	{
		/* Simulate an order chain */
		hyper::protocol_a::schema::NewOrderSingle new_order{.a = 20, .b = 3, .cl_ord_id = 100};
		std::string_view new_order_data{reinterpret_cast<char *>(&new_order), new_order.header.size};
		/* Client sends NewOrderSingle */
		sim.send_request_to_source_session(new_order_data);
		if (!sim.is_msg_received_by_destination()) [[unlikely]]
			return 0;
		/* Venue received NewOrderSingle */
		auto &dest_new = sim.get_last_msg<hyper::protocol_b::schema::NewOrderSingle>();
		std::cout << "venue received: " << dest_new << std::endl;

		/* Venue sends ExecutionReport */
		hyper::protocol_b::schema::ExecutionReport exec{.a = 5, .b = 9, .c = dest_new.c};
		std::string_view exec_data{reinterpret_cast<char *>(&exec), exec.header.size};
		sim.send_response_to_destination_session(exec_data);

		if (!sim.is_msg_received_by_source()) [[unlikely]]
			return 0;
		/* Client received ExecutionReport */
		auto &src_exec = sim.get_last_msg<hyper::protocol_a::schema::ExecutionReport>();
		std::cout << "client received: " << src_exec << std::endl;

		/* Client sends CancelReplaceRequest */
		hyper::protocol_a::schema::CancelReplaceRequest amend{.a = 30, .b = 5, .cl_ord_id = 101, .orig_cl_ord_id = new_order.cl_ord_id};
		std::string_view amend_data{reinterpret_cast<char *>(&amend), amend.header.size};
		sim.send_request_to_source_session(amend_data);

		if (!sim.is_msg_received_by_source()) [[unlikely]]
			return 0;
		/* Venue received CancelReplaceRequest */
		auto &dst_amend = sim.get_last_msg<hyper::protocol_b::schema::CancelReplaceRequest>();
		std::cout << "venue received: " << dst_amend << std::endl;
	}

	return 0;
}