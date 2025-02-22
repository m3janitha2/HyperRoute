#pragma once

#include <variant>
#include <memory>
#include <examples/trading_system/validator/ValidatorX.h>

namespace hyper
{
    using ValidatorPtrVariant = std::variant<std::shared_ptr<ValidatorX>>;
};