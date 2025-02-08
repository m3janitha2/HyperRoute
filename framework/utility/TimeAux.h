#pragma once

#include <chrono>

namespace hyper::framework
{
    using TimestampClock = std::chrono::steady_clock;
    using Timestamp = std::chrono::time_point<TimestampClock>;
}