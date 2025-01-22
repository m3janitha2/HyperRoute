#pragma once

#include <variant>
#include <memory>

namespace hyper
{
    class ValidatorX;
    
    using ValidatorPtrVariant = std::variant<std::shared_ptr<ValidatorX>>;
};