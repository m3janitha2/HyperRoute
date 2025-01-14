#pragma once

#include <variant>
#include <examples/trading_system/validator/ValidatorX.h>

namespace hyper
{
    using ValidatorPtrVarient = std::variant<ValidatorX *>;
};