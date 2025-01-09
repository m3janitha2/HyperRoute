#pragma once

#include <variant>
#include <application/validator/ValidatorX.h>

namespace hyper
{
    using ValidatorPtrVarient = std::variant<ValidatorX*>;
};