#pragma once

#include <framework/router/DestinationRouter.h>
#include <framework/application_dependency/DestinationSessions.h>

namespace hyper::framework
{
	/* Route messages from the source to the destination session. */
	/* Reject the message if destination session is disconnected */
	class DestinationRouterOneToOne
	{
	public:
		explicit DestinationRouterOneToOne(const DestinationSessionPtrVarient &destination_session)
			: destination_session_(destination_session) {}

		DestinationRouterOneToOne(const DestinationRouterOneToOne &) = delete;
		DestinationRouterOneToOne &operator=(const DestinationRouterOneToOne &) = delete;

		template <typename Msg>
		RejectInfo on_message_from_source(Msg &msg) noexcept
			requires RouterMsg<Msg>
		{
			return send_message_to_desination(msg);
		}

	private:
		template <typename Msg>
		RejectInfo send_message_to_desination(Msg &msg) noexcept;

		const DestinationSessionPtrVarient &destination_session_;
	};

	template <typename Msg>
	inline RejectInfo DestinationRouterOneToOne::send_message_to_desination(Msg &msg) noexcept
	{
		return std::visit([&msg]<typename Destination>(Destination &&destination)
							  requires RouterDestination<Destination, Msg>
						  { if(!std::forward<Destination>(destination)->is_connected()) [[unlikely]]
									return RejectInfo{"", InteranlRejectCode::DestinationRouter_No_Destinations_Available};
								else
									return std::forward<Destination>(destination)->on_message_from_peer(msg); },
						  destination_session_);
	}
}