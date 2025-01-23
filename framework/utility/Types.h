#pragma once

#include <cstdint>
#include <iostream>

namespace hyper::framework
{
    using UID = std::uint64_t;
}

template <typename Msg>
inline void log_message_info(Msg &msg)
{
    std::cout << "Message to transport: " << msg
              << " in timestamp:" << msg.in_timestamp().time_since_epoch().count()
              << " out timestamp:" << msg.out_timestamp().time_since_epoch().count()
              << " latency in ns:" << msg.latency_in_ns() << std::endl;
}