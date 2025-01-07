#pragma once
#include <ostream>
#include <concepts>

namespace hyper::framework
{
    template <typename ErrorCode>
    concept ErroCodeType = requires {
        ErrorCode::Success;
    };

    template <ErroCodeType ErrorCode>
    class [[nodiscard]] ErrorInfo
    {
    public:
        constexpr ErrorInfo() = default;
        constexpr ErrorInfo(const char *reason, ErrorCode code = ErrorCode::Success) noexcept
            : code_(code), reason_(reason) {}

        [[nodiscard]] constexpr operator bool() const noexcept { return code_ == ErrorCode::Success; }

        template <typename T>
        friend std::ostream &operator<<(std::ostream &os, const ErrorInfo<T> &error_info);

    private:
        ErrorCode code_{ErrorCode::Success};
        const char *reason_{""};
    };

    template <typename T>
    inline std::ostream &operator<<(std::ostream &os, const ErrorInfo<T> &error_info)
    {
        os << "code:" << to_chars(error_info.code_) << " reason:" << error_info.reason_;
        return os;
    }
}