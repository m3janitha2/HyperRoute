#pragma once
#include <framework/config/Configuration.h>
#include <framework/transport/TransportCallbacks.h>
#include <framework/utility/RejectInfo.h>
#include <framework/transport/TransportSingleThreaded.h>
#include <framework/transport/TransportAPI.h>
#include <string_view>

namespace hyper::framework
{
    using Transport = TransportSingleThreaded;
}
