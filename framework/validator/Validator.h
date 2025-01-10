#pragma once

#include <framework/utility/CrtpBase.h>
#include <framework/utility/RejectInfo.h>
#include <concepts>

namespace hyper::framework
{
    /* Example of a validator */
    template <typename ValidatorImpl>
    class Validator : public CrtpBase<ValidatorImpl>
    {
    public:
        Validator() = default;

        Validator(const Validator &) = delete;
        Validator &operator=(const Validator &) = delete;

        template <typename Msg>
        RejectInfo validate(Msg &msg) { return this->impl().validate_impl(msg); }
    };
}