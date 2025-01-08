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
              << " in timestamp:" << msg.in_timestamp()
              << " out timestamp:" << msg.out_timestamp()
              << " latency in ns:" << msg.latency_in_ns() << std::endl;
}