#pragma once

#include <string>
#include <filesystem>
#include <fstream>
#include <boost/interprocess/file_mapping.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <iostream>

namespace hyper::framework {
    namespace bip = boost::interprocess;

    template <typename Data>
    class MemoryMappedData {
    public:
        explicit MemoryMappedData(const std::string &filename) {
            initialize_memory_mapping(filename);
        }

        ~MemoryMappedData() {
            memory_region_.flush();
        }

        Data &get_mapped_data() noexcept { return *mapped_data_; }

    private:
        void initialize_memory_mapping(const std::string &filename) {
            bool file_exists = std::filesystem::exists(filename);
            
            if (!file_exists)
                create_empty_file(filename, sizeof(Data));

            bip::file_mapping mapped_file(filename.c_str(), bip::read_write);
            memory_region_ = bip::mapped_region(mapped_file, bip::read_write, 0, sizeof(Data));
            mapped_data_ = static_cast<Data *>(memory_region_.get_address());

            if (!file_exists)
                mapped_data_->initialize();
        }

        static void create_empty_file(const std::string &filename, std::size_t size) {
            std::cerr << "Creating new file: " << filename << "\n";
            std::ofstream file(filename, std::ios::binary | std::ios::out);
            file.seekp(size - 1);
            file.write("", 1);
            file.close();
        }

        bip::mapped_region memory_region_;
        Data *mapped_data_{nullptr};
    };
}
