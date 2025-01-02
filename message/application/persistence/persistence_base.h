#pragma once

#include <CrtpWrapper.h>
#include <string_view>

namespace core
{    
    template <typename T>
    class persistance_base : public crtp_base<T>
    {
        void persist(std::string_view data, std::uint64_t seq_no)
        {
            this->impl().persist_impl();
        }
    };
}