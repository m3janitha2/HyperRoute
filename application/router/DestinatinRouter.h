#pragma once

#include <cstdint>
#include <concepts>

namespace max
{
	template <typename DestinationSession>
	concept DestinatinRouter = requires(DestinationSession vs) {
		vs.on_message_from_source();
	};

	// class DestinatinRouter
	// {
	// public:
	// 	template <typename SourceMsg>
	// 	void on_message_from_source(SourceMsg &msg)
	// 	{
	// 		venue_.on_request(msg);
	// 	}

	// 	DestinationSession &venue_;
	// };
}