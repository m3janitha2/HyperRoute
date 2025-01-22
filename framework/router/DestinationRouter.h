#pragma once

#include <framework/utility/CrtpBase.h>
#include <framework/utility/RejectInfo.h>
#include <framework/message/Message.h>
#include <framework/config/Configuration.h>
#include <framework/application_dependency/DestinationProtocolDeclarations.h>
#include <type_traits>
#include <concepts>

namespace hyper::framework
{
	/* DestinationRouter interface */
	template <typename RouterImpl, typename Msg>
	concept DestinationRouterInf = requires(RouterImpl r, Msg msg) {
		/* figureout how to pass MessageInf as Msg ?? */
		{ r.on_message_from_source(msg) } -> std::same_as<RejectInfo>;
	};

	using DestinationProtocolByUid = std::unordered_map<std::size_t, DestinationProtocolPtrVariant>;

	template <typename RouterImpl>
	class DestinationRouter : public CrtpBase<RouterImpl>
	{
	public:
		DestinationRouter() = default;

		DestinationRouter(const DestinationRouter &) = delete;
		DestinationRouter &operator=(const DestinationRouter &) = delete;

		template <MessageInf Msg>
		RejectInfo on_message_from_source(Msg &msg) noexcept
		{
			return this->impl().on_message_from_source_impl(msg);
		}
	};

	template <typename DestinationSession, typename Msg>
	concept RouterDestinationSessionInf = requires(DestinationSession ds, Msg msg) {
		ds->on_message_from_peer(msg);
		ds->is_connected();
	};
}