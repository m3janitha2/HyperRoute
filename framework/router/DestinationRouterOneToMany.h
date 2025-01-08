#pragma once

#include <framework/utility/RejectInfo.h>
#include <framework/message/Message.h>
#include <framework/router/DestinationRouter.h>
#include <framework/application_dependency/DestinationSessions.h>
#include <cstdint>
#include <vector>
#include <iostream>

namespace hyper::framework
{
	/* Route messages from the source session to destination sessions using a round-robin pattern */
	/* Attempt the next available session if the current session is not connected */
	/* Reject the message if all sessions are disconnected */
	class DestinationRouterOneToMany
	{
	public:
		explicit DestinationRouterOneToMany(std::vector<DestinationSessionPtrVarient *> &destinations) : destinations_{destinations} {}

		template <typename Msg>
		RejectInfo on_message_from_source(Msg &msg) noexcept
			requires RouterMsg<Msg>
		{
			if (auto reject_info = send_message_to_desination(msg); reject_info != true)
				return reject_info;

			return send_message_to_desination(msg);
		}

		auto next_session_index() noexcept
		{
			return index_++ % destinations_.size();
		}

		template <typename Msg>
		RejectInfo send_message_to_desination(Msg &msg) noexcept
		{
			if constexpr (std::is_base_of_v<Msg, framework::message::FirstEvent>)
			{
				auto next_index = next_session_index();
				auto &next_destination = *destinations_[next_index];
				auto reject_info = send_message_to_desination(msg, next_destination);

				/* Message sent out from the destination session */
				insert_destination_by_uid(msg.uid(), next_destination);
				return reject_info;
			}
			else
			{
				try
				{
					auto uid = msg.uid();
					if (auto it = uid_to_destination_.find(uid); it != uid_to_destination_.end())
					{
						auto &destination = it->second;
						return send_message_to_desination(msg, destination);
					}
					else
						return RejectInfo{"Destination session not found for uid", InteranlRejectCode::Destination_Session_Not_Found_For_UID};
				}
				catch (const std::exception &error)
				{
					std::cout << error.what() << std::endl;
					return RejectInfo{"Exception. Destination session not found for uid", InteranlRejectCode::Destination_Session_Not_Found_For_UID};
				}
			}
		}

		template <typename Msg>
		RejectInfo send_message_to_desination(Msg &msg, DestinationSessionPtrVarient &destination_session) const noexcept
		{
			return std::visit([&msg]<typename Destination>(Destination &&destination)
								  requires RouterDestination<Destination, Msg>
							  { if(!std::forward<Destination>(destination)->is_connected()) [[unlikely]]
									return RejectInfo{"Destination is not connected", InteranlRejectCode::Destination_Is_Not_Connected};
								else
									return std::forward<Destination>(destination)->on_message_from_peer(msg); },
							  destination_session);
		}

	private:
		void insert_destination_by_uid(UID uid, DestinationSessionPtrVarient &destination_session)
		try
		{
			if (auto [it, ret] = uid_to_destination_.emplace(uid, destination_session); ret != true)
			{
				std::cout << "Crtical Error" << std::endl;
			}
		}
		catch (std::exception &error)
		{
			std::cout << "Crtical Error: " << error.what();
		}

		std::vector<DestinationSessionPtrVarient *> &destinations_;
		std::size_t index_{0};
		std::unordered_map<UID, DestinationSessionPtrVarient &> uid_to_destination_{};
	};
}