#pragma once

#include <cstdint>
#include <concepts>

namespace hyper::framework
{
	template <typename Msg>
	concept RouterMsg = requires (Msg msg){
		msg.uid();
	};

	template <typename DestinationSession, typename Msg>
	concept RouterDestination = requires (DestinationSession ds, Msg msg)
	{
		ds->on_message_from_peer(msg);
	};
}