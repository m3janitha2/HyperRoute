#pragma once

#include <string>
#include <filesystem>
#include <fstream>
#include <boost/interprocess/file_mapping.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <iostream>

namespace hyper::framework
{
    namespace bip = boost::interprocess;

    template <typename Data>
    class MemoryMappedData
    {
    public:
        explicit MemoryMappedData(const std::string &filename)
        {
            bool file_exists = is_file_exist(filename, sizeof(Data));
            bip::file_mapping mapped_file(filename.c_str(), bip::read_write);
            mapped_region_ = bip::mapped_region(mapped_file, bip::read_write, 0, sizeof(Data));
            data_ = static_cast<Data *>(mapped_region_.get_address());

            if (!file_exists)
                data_->initialize();
        }

        ~MemoryMappedData()
        {
            mapped_region_.flush();
        }

        Data &data() noexcept { return *data_; }

    private:
        static bool is_file_exist(const std::string &filename, std::size_t size)
        {
            if (std::filesystem::exists(filename))
            {
                return true;
            }

            std::cout << "Creating new file: " << filename << "\n";
            std::ofstream file(filename, std::ios::binary | std::ios::out);
            file.seekp(size - 1);
            file.write("", 1);
            file.close();
            return false;
        }

        bip::mapped_region mapped_region_;
        Data *data_{nullptr};
    };
}
