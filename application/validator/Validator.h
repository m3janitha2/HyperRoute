#pragma once
#include <framework/protocol/RejectInfo.h>

namespace max
{
    using RejectInfo = framework::RejectInfo;

    class Validator
    {
    public:
        template <typename Msg>
        RejectInfo validate(Msg &msg) { return RejectInfo{}; }
    };
}