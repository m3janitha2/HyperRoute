
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

    inline void register_all_validators()
    {
        auto &factory = ValidatorFactory::instance();

        factory.register_type("ValidatorX",
                              [](const Configuration &config)
                              {
                                  return std::make_shared<ValidatorX>(config);
                              });
    }
}
