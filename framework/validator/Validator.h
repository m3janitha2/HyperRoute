#pragma once

#include <framework/utility/CrtpBase.h>
#include <framework/utility/RejectInfo.h>
#include <framework/config/Configuration.h>
#include <concepts>

namespace hyper::framework
{
    /* Example of a validator */
    template <typename ValidatorImpl>
    class Validator : public CrtpBase<ValidatorImpl>
    {
    public:
        explicit Validator(const Configuration &config)
            : config_(config) {}

        Validator(const Validator &) = delete;
        Validator &operator=(const Validator &) = delete;

        template <typename Msg>
        RejectInfo validate(Msg &msg)
        {
            return this->impl().validate_impl(msg);
        }

    private:
        const Configuration &config_;
    };
}