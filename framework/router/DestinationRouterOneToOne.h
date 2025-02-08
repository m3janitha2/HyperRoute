#pragma once

#include <framework/router/DestinationRouter.h>
#include <framework/application_dependency/DestinationSessions.h>
#include <framework/application_dependency/DestinationProtocols.h>

namespace hyper::framework
{
	/* Route messages from the source to the destination session. */
	/* Reject the message if destination session is disconnected */
	class DestinationRouterOneToOne : public DestinationRouter<DestinationRouterOneToOne>
	{
	public:
		explicit DestinationRouterOneToOne(const Configuration &config,
										   const DestinationProtocolByUid &destinations)
		{
			auto id = config.get<std::size_t>("destination_session_id");
			auto &protocol = destinations.at(id);
			destination_session_ = &(std::visit([]<typename Protocol>(Protocol &p) -> decltype(auto)
												{ return p->session(); },
												protocol));
		}

		DestinationRouterOneToOne(const DestinationRouterOneToOne &) = delete;
		DestinationRouterOneToOne &operator=(const DestinationRouterOneToOne &) = delete;

		template <typename Msg>
		RejectInfo on_message_from_source_impl(Msg &msg) const noexcept
		{
			return send_message_to_desination(msg);
		}

	private:
		template <typename Msg>
		RejectInfo send_message_to_desination(Msg &msg) const noexcept;

		DestinationSessionPtrVariant destination_session_;
	};

	template <typename Msg>
	inline RejectInfo DestinationRouterOneToOne::send_message_to_desination(Msg &msg) const noexcept
	{
		return std::visit([&msg]<typename Destination>(Destination &&destination_session)
						  { if(!std::forward<Destination>(destination_session)->is_connected()) [[unlikely]]
									return RejectInfo{"", InteranlRejectCode::DestinationRouter_No_Destinations_Available};
								else
									return std::forward<Destination>(destination_session)->on_message_from_peer(msg); },
						  destination_session_);
	}
}