#pragma once
#include <cstdint>
#include <ostream>
namespace max
{
    enum class InteranlRejectCode
    {
        None,
        Client_Enrichment_Failed,
        Client_Validation_Failed,
        To_Venue_Routing_Failed,
        To_Venue_Validation_Failed,
        To_Venue_Enrichement_Failed,
        To_Venue_Encoding_Failed,
        From_Venue_Decoding_Failed,
        To_Client_Routing_Failed,
    };

    inline const char *to_chars(InteranlRejectCode code)
    {
        using enum InteranlRejectCode;
        switch (code)
        {
        case Client_Enrichment_Failed:
            return "Client_Enrichment_Failed";
        case Client_Validation_Failed:
            return "Client_Validation_Failed";
        case To_Venue_Routing_Failed:
            return "To_Venue_Routing_Failed";
        case To_Venue_Validation_Failed:
            return "To_Venue_Validation_Failed";
        case To_Venue_Enrichement_Failed:
            return "To_Venue_Enrichement_Failed";
        case To_Venue_Encoding_Failed:
            return "To_Venue_Encoding_Failed";
        case From_Venue_Decoding_Failed:
            return "From_Venue_Decoding_Failed";
        case To_Client_Routing_Failed:
            return "To_Client_Routing_Failed";
        default:
            return "Unknown";
        }
    }

    inline std::string to_string(InteranlRejectCode code)
    {
        return to_chars(code);
    }

    template <typename RejectCode>
    class RejectInfoBase
    {
    public:
        constexpr RejectInfoBase() = default;
        constexpr RejectInfoBase(const char *reason, RejectCode code = RejectCode::None) noexcept
            : code_(code), reason_(reason) {}

        constexpr operator bool() const noexcept { return code_ == RejectCode::None; }
        const char *to_chars() const { return to_chars(code_); }

        template <typename T>
        friend std::ostream &operator<<(std::ostream &os, const RejectInfoBase<T> &reject_info);

            private : RejectCode code_ { RejectCode::None };
        const char *reason_{""};
    };

    template <typename T>
    inline std::ostream &operator<<(std::ostream &os, const RejectInfoBase<T> &reject_info)
    {
        //"code:" << reject_info.to_chars() << " reason:" << reject_info.reason_;
        return os;
    }

    using RejectInfo = RejectInfoBase<InteranlRejectCode>;
}