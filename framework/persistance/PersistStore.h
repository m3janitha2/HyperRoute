#pragma once

#include <CrtpBase.h>
#include <string_view>

namespace hyper::framework
{
    /* todox: Persisted store abstraction. */

    template <typename PersistStoreImpl>
    class PersistStore : public CrtpBase<PersistStoreImpl>
    {
        void persist(std::uint64_t seq_no, std::string_view data)
        {
            this->impl().persist_impl(seq_no, data);
        }
    };
}