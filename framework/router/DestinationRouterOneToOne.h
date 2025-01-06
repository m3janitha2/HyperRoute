#pragma once

#include <application/session/DestinationSessions.h>

namespace max::framework
{
	class DestinationRouterOneToOne
	{
	public:
		explicit DestinationRouterOneToOne(DestinationSessionPtrVarient &destination_session)
			: destination_session_(destination_session) {}

		DestinationRouterOneToOne(const DestinationRouterOneToOne&) = delete;
		DestinationRouterOneToOne& operator=(const DestinationRouterOneToOne&) = delete;

		template <typename Msg>
		RejectInfo on_message_from_source(Msg &msg) noexcept
		{
			return send_message_to_desination(msg);
		}

	private:
		template <typename Msg>
		RejectInfo send_message_to_desination(Msg &msg) noexcept
		{
			return std::visit([&msg](auto &&destination_session)
							  { return destination_session->on_message_from_peer(msg); },
							  destination_session_);
		}

		DestinationSessionPtrVarient &destination_session_;
	};
}