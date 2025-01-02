#pragma once

#include <cstdint>
#include <concepts>

namespace max
{
	template <typename VenueSession>
	concept ForwardRouter = requires {
		VenueSession::on_client_msg();
	};

	// template <typename VenueSession>
	// class ForwardRouterBase
	// {
	// public:
	// 	template <typename ClientMsg>
	// 	void on_message_from_client(ClientMsg &msg)
	// 	{
	// 		venue_.on_request(msg);
	// 	}

	// 	VenueSession &venue_;
	// };
}