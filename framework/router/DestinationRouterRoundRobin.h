#pragma once

#include <framework/router/DestinationRouter.h>
#include <cstdint>
#include <vector>
#include <iostream>

namespace hyper::framework
{
	/* Route messages from the source session to destination sessions using a round-robin pattern */
	/* Attempt the next available session if the current session is not connected */
	/* Reject the message if all sessions are disconnected */
	class DestinationRouterRoundRobin : public DestinationRouter<DestinationRouterRoundRobin>
	{
	public:
		// explicit DestinationRouterRoundRobin(const std::vector<DestinationSessionPtrVarient *> &destinations)
		// 	: destinations_{destinations} {}

		explicit DestinationRouterRoundRobin(const Configuration &config,
											 const DestinationProtocolByUid &destinations)
		{
			for (const auto &node : config)
			{
				if (node.first == "destination_session_id")
				{
					auto id = node.second.get_value<std::size_t>();
					auto &protocol = destinations.at(id);
					DestinationSessionPtrVarient session = &(std::visit([]<typename Protocol>(Protocol &p) -> decltype(auto)
																		{ return p->session(); },
																		protocol));
					destinations_.emplace_back(session);
				}
			}
		}

		DestinationRouterRoundRobin(const DestinationRouterRoundRobin &) = delete;
		DestinationRouterRoundRobin &operator=(const DestinationRouterRoundRobin &) = delete;

		template <typename Msg>
		RejectInfo on_message_from_source_impl(Msg &msg) noexcept
		{
			if (auto reject_info = send_message_to_desination(msg);
				reject_info != true) [[unlikely]]
				return reject_info;

			return RejectInfo{};
		}

	private:
		auto &get_next_session() noexcept;
		template <typename Msg>
		RejectInfo send_message_to_desination(Msg &msg) noexcept;
		template <typename Msg>
		RejectInfo send_first_event_to_next_available_desination(Msg &msg) noexcept;
		template <typename Msg>
		RejectInfo send_subsequent_events_to_the_same_desination(Msg &msg) noexcept;
		bool is_destination_connected(const DestinationSessionPtrVarient &destination_session) const noexcept;
		template <typename Msg>
		RejectInfo send_message_to_desination(Msg &msg, const DestinationSessionPtrVarient &destination_session) const noexcept;
		void cache_destination_by_uid(UID uid, DestinationSessionPtrVarient &destination_session) noexcept;

		// const std::vector<DestinationSessionPtrVarient *> &destinations_;
		std::vector<DestinationSessionPtrVarient> destinations_;
		decltype(destinations_.size()) index_{0};
		std::unordered_map<UID, DestinationSessionPtrVarient &> uid_to_destination_{};
	};

	inline auto &DestinationRouterRoundRobin::get_next_session() noexcept
	{
		index_ = (index_ + 1) % destinations_.size();
		return destinations_[index_];
	}

	template <typename Msg>
	inline RejectInfo DestinationRouterRoundRobin::send_message_to_desination(Msg &msg) noexcept
	{
		if constexpr (std::derived_from<Msg, FirstEvent>)
			return send_first_event_to_next_available_desination(msg);
		else
			return send_subsequent_events_to_the_same_desination(msg);
	}

	template <typename Msg>
	inline RejectInfo DestinationRouterRoundRobin::send_first_event_to_next_available_desination(Msg &msg) noexcept
	{
		for (decltype(destinations_.size()) i{0}; i < destinations_.size(); i++)
		{
			auto &destination = get_next_session();
			if (is_destination_connected(destination))
			{
				auto reject_info = send_message_to_desination(msg, destination);
				/* Message sent out from the destination session */
				cache_destination_by_uid(msg.uid(), destination);
				return reject_info;
			}
		}
		return RejectInfo{"", InteranlRejectCode::DestinationRouter_No_Destinations_Available};
	}

	template <typename Msg>
	inline RejectInfo DestinationRouterRoundRobin::send_subsequent_events_to_the_same_desination(Msg &msg) noexcept
	try
	{
		auto uid = msg.uid();
		if (auto it = uid_to_destination_.find(uid);
			it != uid_to_destination_.end()) [[likely]]
		{
			auto &destination = it->second;
			return send_message_to_desination(msg, destination);
		}
		else
			return RejectInfo{"Destination session not found for uid", InteranlRejectCode::DestinationRouter_Session_Not_Found_For_UID};
	}
	catch (const std::exception &error)
	{
		std::cout << error.what() << std::endl;
		return RejectInfo{"Exception. Destination session not found for uid", InteranlRejectCode::DestinationRouter_Session_Not_Found_For_UID};
	}

	inline bool DestinationRouterRoundRobin::is_destination_connected(const DestinationSessionPtrVarient &destination_session) const noexcept
	{
		return std::visit([]<typename Destination>(Destination &&destination)
						  { return std::forward<Destination>(destination)->is_connected(); },
						  destination_session);
	}

	template <typename Msg>
	inline RejectInfo DestinationRouterRoundRobin::send_message_to_desination(Msg &msg, const DestinationSessionPtrVarient &destination_session) const noexcept

	{
		return std::visit([&msg]<typename Destination>(Destination &&destination)
							  requires RouterDestinationSessionInf<Destination, Msg>
						  { return std::forward<Destination>(destination)->on_message_from_peer(msg); },
						  destination_session);
	}

	inline void DestinationRouterRoundRobin::cache_destination_by_uid(UID uid, DestinationSessionPtrVarient &destination_session) noexcept
	try
	{
		if (auto [it, ret] = uid_to_destination_.emplace(uid, destination_session);
			ret != true) [[unlikely]]
			std::cout << "Crtical Error" << std::endl;
	}
	catch (std::exception &error)
	{
		std::cout << "Exception. Crtical Error: " << error.what();
	}
}