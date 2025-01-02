#pragma once
#include <application/session/RejectInfo.h>

namespace max
{
    class Validator
    {
    public:
        template <typename Msg>
        RejectInfo validate(Msg &msg) { return RejectInfo{}; }
    };
}