#pragma once

#include <CrtpBase.h>
#include <string_view>

namespace hyper::framework
{    
    template <typename T>
    class Persistance : public CrtpBase<T>
    {
        void persist(std::uint64_t seq_no, std::string_view data)
        {
            this->impl().persist_impl();
        }
    };
}