#pragma once

#include <cstdint>
#include <concepts>
#include <application/session/VenueSessions.h>

namespace max
{
	class ForwardRouterOneToOne
	{
	public:
		explicit ForwardRouterOneToOne(VenueSessionVarient &venue_session)
			: venue_session_(venue_session) {}

		template <typename Msg>
		RejectInfo on_message_from_client(Msg &msg)
		{
			return send_message_to_venue(msg);
		}

		template <typename Msg>
		RejectInfo send_message_to_venue(Msg &msg)
		{
			return std::visit([&msg](auto &&venue_session)
							  { return venue_session.on_client_message(msg); },
							  venue_session_);
		}

	private:
		VenueSessionVarient &venue_session_;
	};
}