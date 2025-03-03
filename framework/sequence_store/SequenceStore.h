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
            : m_mapped_data_(decorate_file_name(filename)),
              data_(m_mapped_data_.data()) {}

        // Input sequence number methods
        SeqNumType in_sequence_number() const { return data_.in_sequence_number_; }
        SeqNumType next_in_sequence_number() { return ++data_.in_sequence_number_; }
        void in_sequence_number(SeqNumType seq) { data_.in_sequence_number_ = seq; }

        // Output sequence number methods
        SeqNumType out_sequence_number() const { return data_.out_sequence_number_; }
        SeqNumType next_out_sequence_number() { return ++data_.out_sequence_number_; }
        void out_sequence_number(SeqNumType seq) { data_.out_sequence_number_ = seq; }

    private:
        static auto decorate_file_name(const std::string &filename)
        {
            return filename + "_seq.dat";
        }

        struct Data
        {
            SeqNumType in_sequence_number_{0};
            SeqNumType out_sequence_number_{0};

            void initialize()
            {
                in_sequence_number_ = 0;
                out_sequence_number_ = 0;
            }
        };

        MemoryMappedData<Data> m_mapped_data_;
        Data &data_;
    };
}
