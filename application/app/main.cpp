
#include <application/router/ForwardRouters.h>
#include <application/session/VenueSessions.h>
#include <application/config/Configuration.h>
#include <application/router/ReverseRouter.h>
#include <application/session/ProtocolA.h>
#include <application/session/ProtocolB.h>
#include <application/session/VenueSessionProtocolB.h>

using namespace max;

struct SubSystem
{
	ReverseRouter reverse_router{};
	protocol_b::ProtocolB venue_protocol{reverse_router};
	const VenueSessionVarient &venue_session_varient{venue_protocol.session()};
	ForwardRouterOneToOne forwar_router{const_cast<VenueSessionVarient &>(venue_session_varient)};
	ForwardRouterVarient forward_router_variant{forwar_router};
	protocol_a::ProtocolA client_protocol{forward_router_variant, reverse_router};
};

int main(int argc, char **argv)
{
	Configuration cfg{"/mnt/d/linux/github/max/application/config/config.xml"};
	auto x = cfg.get_client_ses_cfg_by_name("ClientA");
	SubSystem ss{};

	protocol_a::schema::NewOrderSingle new_order{.a = 20, .b = 3};
	auto *new_order_ptr = reinterpret_cast<char *>(&new_order);
	std::string_view data{new_order_ptr, sizeof(new_order)};
	ss.client_protocol.on_data(data);

	protocol_b::schema::ExecutionReport exec{.a = 5, .b = 9};
	auto *exec_ptr = reinterpret_cast<char *>(&exec);
	std::string_view data2{exec_ptr, sizeof(new_order)};
	ss.venue_protocol.on_data(data2);
	return 0;
}