
#pragma once

#include <framework/factory/Factory.h>
#include <framework/config/Configuration.h>
#include <framework/application_dependency/ValidatorDeclarations.h>

namespace hyper::framework
{
    using ValidatorCreator =
        std::function<ValidatorPtrVariant(const Configuration &config)>;

    using ValidatorFactory = Factory<ValidatorCreator>;

    template <typename Type>
    inline void register_validator(const std::string &key)
    {
        auto &factory = ValidatorFactory::instance();
        factory.register_type(
            key,
            [](const Configuration &config)
            {
                return std::make_shared<Type>(config);
            });
    }
}
