#pragma once
#include <cstdint>

namespace ouch
{
	struct cancel_ack
	{
		std::uint32_t id{};
	};
}