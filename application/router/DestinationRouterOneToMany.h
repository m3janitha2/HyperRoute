#pragma once

#include <application/session/DestinationSessions.h>
#include <application/session/RejectInfo.h>
#include <application/message/core/OrderStore.h>
#include <cstdint>
#include <concepts>
#include <vector>
#include <iostream>

namespace max
{
	class DestinationRouterOneToMany
	{
	public:
		explicit DestinationRouterOneToMany(std::vector<DestinationSessionVarient &> destinations) : destinations_{destinations} {}

		template <typename Msg>
		RejectInfo on_message_from_source(Msg &msg)
		{
			if (auto reject_info = send_message_to_desination(msg); reject_info != true)
				return reject_info;

			return send_message_to_desination(msg);
		}

		auto next_session_index()
		{
			return index_++ % destinations_.size();
		}

		template <typename Msg>
		RejectInfo send_message_to_desination(Msg &msg)
		{
			if constexpr (std::is_same_v<Msg, core::NewOrderSingle>)
			{
				auto next_index = next_session_index();
				auto &next_destination = destinations[next_index];
				auto reject_info = send_message_to_desination(msg, next_destination);
				/* Message sent out from the destination session */
				insert_destination_by_uid(msg.uid(), next_destination);
				return reject_info;
			}
			else
			{
				auto uid = msg.uid();
				if (auto [reject_info, destination] = find_destination_by_uid(); reject_info != true) [[unlikely]]
					return reject_info;
				else [[likely]]
					return send_message_to_desination(msg, destination);
			}
		}

		template <typename Msg>
		RejectInfo send_message_to_desination(Msg &msg, DestinationSessionVarient &destination_session)
		{
			return std::visit([&msg](auto &&destination_session)
							  { return destination_session.on_message_from_peer(msg); },
							  destination_session);
		}

	private:
		void insert_destination_by_uid(UID uid, DestinationSessionVarient &destination_session)
		{
			if (auto [it, ret] = uid_to_destination_.emplace(uid, destination_session); ret != true)
			{
				std::cout << "Crtical Error" << std::endl;
			}
		}

		auto find_destination_by_uid(UID uid)
		{
			if (auto it = uid_to_destination_.find(uid); it != uid_to_destination_.end())
				return {RejectInfo{}, it->second};
			else
				return {RejectInfo{"Destination session not found for uid", InteranlRejectCode::Destination_Session_Not_Found_For_UID}, DestinationSessionVarient{}};
		}

		std::vector<DestinationSessionVarient &> &destinations_;
		decltype(destinations_.size()) index_{0};
		std::unordered_map<UID, DestinationSessionVarient &> uid_to_destination_{};
	};
}