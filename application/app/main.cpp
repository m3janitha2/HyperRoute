
#include <application/router/DestinatinRouters.h>
#include <application/session/DestinationSessions.h>
#include <application/config/Configuration.h>
#include <application/router/ReverseRouter.h>
#include <application/session/ProtocolA.h>
#include <application/session/ProtocolB.h>
#include <application/session/DestinationSessionProtocolB.h>

using namespace max;

struct SubSystem
{
	ReverseRouter reverse_router{};
	protocol_b::ProtocolB venue_protocol{reverse_router};
	const DestinationSessionVarient &destination_session_varient{venue_protocol.session()};
	DestinationRouterOneToOne forwar_router{const_cast<DestinationSessionVarient &>(destination_session_varient)};
	DestinatinRouterVarient forward_router_variant{forwar_router};
	protocol_a::ProtocolA client_protocol{forward_router_variant, reverse_router};
};

int main(int argc, char **argv)
{
	Configuration cfg{"/mnt/d/linux/github/max/application/config/config.xml"};
	auto x = cfg.get_client_ses_cfg_by_name("ClientA");
	SubSystem ss{};

	protocol_a::schema::NewOrderSingle new_order{.a = 20, .b = 3, .cl_ord_id = 100};
	auto *new_order_ptr = reinterpret_cast<char *>(&new_order);
	std::string_view new_order_data{new_order_ptr, new_order.header.size};
	ss.client_protocol.on_data(new_order_data);

	protocol_a::schema::CancelReplaceRequest amend{.a = 30, .b = 5, .cl_ord_id = 101, .orig_cl_ord_id = 100};
	auto *amend_ptr = reinterpret_cast<char *>(&amend);
	std::string_view amend_data{amend_ptr, amend.header.size};
	ss.client_protocol.on_data(amend_data);

	protocol_b::schema::ExecutionReport exec{.a = 5, .b = 9};
	auto *exec_ptr = reinterpret_cast<char *>(&exec);
	std::string_view data2{exec_ptr, sizeof(new_order)};
	ss.venue_protocol.on_data(data2);
	return 0;
}