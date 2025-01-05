#pragma once

#include <cstdint>
#include <concepts>
#include <application/session/DestinationSessions.h>

namespace max
{
	class DestinationRouterOneToOne
	{
	public:
		explicit DestinationRouterOneToOne(DestinationSessionVarient &destination_session)
			: destination_session_(destination_session) {}

		template <typename Msg>
		RejectInfo on_message_from_source(Msg &msg)
		{
			return send_message_to_desination(msg);
		}

	private:
		template <typename Msg>
		RejectInfo send_message_to_desination(Msg &msg)
		{
			return std::visit([&msg](auto &&destination_session)
							  { return destination_session.on_message_from_peer(msg); },
							  destination_session_);
		}

		DestinationSessionVarient &destination_session_;
	};
}