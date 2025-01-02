#pragma once

#include <application/session/VenueSessions.h>
#include <application/session/RejectInfo.h>
#include <cstdint>
#include <concepts>
#include <array>

namespace max
{
	class OneToManyForwardRouter
	{
	public:
		explicit OneToManyForwardRouter(VenueSessionVarient &venue) : venue_(venue) {}

		template <typename Msg>
		RejectInfo on_message_from_client(Msg &msg)
		{
			return send_message_to_venue(msg);
		}

		auto next_session_index()
		{
			return index_++ % sizeof(venues_);
		}

		template <typename Msg>
		RejectInfo send_message_to_venue(Msg &msg)
		{
			return std::visit([&msg, this](auto &venues)
					   {
						auto next_index = next_session_index();
						return venues[next_index].on_client_message(msg); },
					   venues_);
		}

	private:
		std::array<VenueSessionVarient, 5> &venues_;
		std::uint8_t index_{0};
	};
}