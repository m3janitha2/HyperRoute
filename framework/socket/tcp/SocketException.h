#pragma once

#include <stdexcept>
#include <string>
#include <cerrno>
#include <cstring>

namespace hyper::framework
{
    class SocketException : public std::runtime_error
    {
    public:
        explicit SocketException(const std::string &message)
            : std::runtime_error(message) {}

        SocketException(const std::string &message, int err_code)
            : std::runtime_error(message + ": " + std::strerror(err_code)) {}
    };
}