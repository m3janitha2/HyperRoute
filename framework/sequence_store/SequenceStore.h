
#pragma once

/* Persisted sequence number store. */
/* todox: Implement file persistence. */
namespace hyper::framework
{
    template <typename SeqNumType>
    struct SequenceStore
    {
        SeqNumType in_sequence_number_{0};
        SeqNumType out_sequence_number_{0};
    };
}