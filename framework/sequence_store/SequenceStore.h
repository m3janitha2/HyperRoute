#pragma once

#include <framework/sequence_store/MemoryMappedData.h>
#include <string>

namespace hyper::framework
{
    template <typename SeqNumType>
    class SequenceStore
    {
    public:
        explicit SequenceStore(const std::string &filename)
            : mapped_data_(generate_storage_filename(filename)),
              data_(mapped_data_.get_mapped_data()) {}

        SeqNumType in_sequence_number() const { return data_.in_sequence_number_; }
        SeqNumType next_in_sequence_number() { return ++data_.in_sequence_number_; }
        void in_sequence_number(SeqNumType seq) { data_.in_sequence_number_ = seq; }

        SeqNumType out_sequence_number() const { return data_.out_sequence_number_; }
        SeqNumType next_out_sequence_number() { return ++data_.out_sequence_number_; }
        void out_sequence_number(SeqNumType seq) { data_.out_sequence_number_ = seq; }

    private:
        static std::string generate_storage_filename(const std::string &filename)
        {
            return filename + "_seq.dat";
        }

        struct SequenceData
        {
            SeqNumType in_sequence_number_{0};
            SeqNumType out_sequence_number_{0};

            void initialize()
            {
                in_sequence_number_ = 0;
                out_sequence_number_ = 0;
            }
        };

        MemoryMappedData<SequenceData> mapped_data_;
        SequenceData &data_;
    };
}
