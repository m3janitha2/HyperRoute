#pragma once

#include <variant>
#include <example/trading_system/validator/ValidatorX.h>

namespace hyper
{
    using ValidatorPtrVarient = std::variant<ValidatorX *>;
};