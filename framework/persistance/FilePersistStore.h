#pragma once
#include <persistence_base.h>

namespace framework
{
    template <typename T>
    class FilePersistStore : public PersistStore<FilePersistStore>
    {
        void persist_impl(std::uint64_t seq_no, std::string_view data)
        {
           
        }
        /* todox: write to a memory mapped file for session recovery */
    };
}
