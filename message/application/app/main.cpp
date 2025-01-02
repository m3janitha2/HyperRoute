
#include <application/router/ForwardRouters.h>
#include <application/session/VenueSessions.h>
#include <application/session/ClientSessionProtocolA.h>
#include <application/config/Configuration.h>
#include <application/router/ReverseRouter.h>

using namespace max;

template <typename ClientSession, typename ForwardRouter, typename VenueSession>
struct SubSystem

{
	ReverseRouter reverse_router{};
	VenueSession venue_session{reverse_router};
	VenueSessionVarient venue_session_variant{venue_session};
	ForwardRouter forwar_router{venue_session_variant};
	ForwardRouterVarient forward_router_variant{forwar_router};
	ClientSession client{forward_router_variant, reverse_router};
};

int main(int argc, char **argv)
{
	Configuration cfg{"/mnt/d/linux/github/max/application/config/config.xml"};
	auto x = cfg.get_client_ses_cfg_by_name("ClientA");
	SubSystem<ClientSessionProtocolA, ForwardRouterOneToOne, VenueSessionProtocolB> ss{}; 

	ss.client.on_data();
	ss.venue_session.on_data();
	return 0;
}