
#include <framework/router/DestinationRouters.h>
#include <framework/router/SourceRouter.h>
#include <framework/config/Configuration.h>
#include <application/session/DestinationSessions.h>
#include <application/session/ProtocolA.h>
#include <application/session/ProtocolB.h>
#include <application/session/DestinationSessionProtocolB.h>

using namespace hyper;

struct SubSystem
{
	framework::SourceRouter source_router{};
	protocol_b::ProtocolB destination_protocol{source_router};
	const DestinationSessionPtrVarient destination_session_varient{&destination_protocol.session()};
	framework::DestinationRouterOneToOne forwar_router{const_cast<DestinationSessionPtrVarient &>(destination_session_varient)};
	DestinationRouterPtrVarient forward_router_variant{&forwar_router};
	protocol_a::ProtocolA source_protocol{forward_router_variant, source_router};
};

int main(int argc, char **argv)
{
	// Configuration cfg{"/mnt/d/linux/github/m3janitha2/HyperRoute/application/config/config.xml"};
	// auto x = cfg.get_source_ses_cfg_by_name("SourceA");
	SubSystem ss{};

	for (auto i{0}; i < 10; i++)
	{
		protocol_a::schema::NewOrderSingle new_order{.a = 20, .b = 3, .cl_ord_id = 100};
		auto *new_order_ptr = reinterpret_cast<char *>(&new_order);
		std::string_view new_order_data{new_order_ptr, new_order.header.size};
		ss.source_protocol.transport().on_data(new_order_data);

		protocol_a::schema::CancelReplaceRequest amend{.a = 30, .b = 5, .cl_ord_id = 101, .orig_cl_ord_id = 100};
		auto *amend_ptr = reinterpret_cast<char *>(&amend);
		std::string_view amend_data{amend_ptr, amend.header.size};
		ss.source_protocol.transport().on_data(amend_data);

		protocol_b::schema::ExecutionReport exec{.a = 5, .b = 9};
		auto *exec_ptr = reinterpret_cast<char *>(&exec);
		std::string_view data2{exec_ptr, sizeof(new_order)};
		ss.destination_protocol.on_data(data2);
	}

	return 0;
}