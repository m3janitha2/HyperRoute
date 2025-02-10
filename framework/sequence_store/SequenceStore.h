
#pragma once

/* Persisted sequence number store. */
/* todox: Implement file persistence. */
namespace hyper::framework
{
    template <typename SeqNumType>
    class SequenceStore
    {
    public:
        auto in_sequence_number() { return in_sequence_number_; }
        auto next_in_sequence_number() { return ++in_sequence_number_; }
        void in_sequence_number(SeqNumType seq) { in_sequence_number_ = seq; }

        auto out_sequence_number() { return out_sequence_number_; }
        auto next_out_sequence_number() { return ++out_sequence_number_; }
        void out_sequence_number(SeqNumType seq) { out_sequence_number_ = seq; }

    private:
        SeqNumType in_sequence_number_{0};
        SeqNumType out_sequence_number_{0};
    };
}