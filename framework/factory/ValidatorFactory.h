
#pragma once

#include <framework/factory/Factory.h>
#include <framework/application_dependency/Validators.h>
#include <framework/config/Configuration.h>

namespace hyper::framework
{
    class SourceRouter;

    using ValidatorCreator =
        std::function<ValidatorPtrVarient(const Configuration &config)>;

    using ValidatorFactory =
        Factory<ValidatorPtrVarient, ValidatorCreator>;

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

    inline void register_all_validators()
    {
        register_validator<ValidatorX>("ValidatorX");
    }
}
