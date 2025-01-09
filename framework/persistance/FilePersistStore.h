#pragma once

#include <framework/persistance/PersistStore.h>
#include <cstdint>
#include <string_view>

namespace hyper::framework
{
    /* todox: File PersistStore abstraction. */
    class FilePersistStore : public PersistStore<FilePersistStore>
    {
        void persist_impl(std::uint64_t seq_no, std::string_view data)
        {
            /* todox: write to a memory mapped file for session recovery */
        }
    };
}
