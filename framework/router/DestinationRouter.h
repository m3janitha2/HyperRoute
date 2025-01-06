#pragma once

#include <cstdint>
#include <concepts>

namespace max::framework
{
	template <typename DestinationSession>
	concept DestinationRouter = requires(DestinationSession vs) {
		vs.on_message_from_source();
	};

	// class DestinationRouter
	// {
	// public:
	// 	template <typename SourceMsg>
	// 	void on_message_from_source(SourceMsg &msg)
	// 	{
	// 		destination_.on_request(msg);
	// 	}

	// 	DestinationSession &destination_;
	// };
}