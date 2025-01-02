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

    inline std::ostream &operator<<(std::ostream &os, const InteranlRejectCode &code)
    {
        using enum InteranlRejectCode;
        switch (code)
        {
        case Client_Enrichment_Failed:
            return os << "Client_Enrichment_Failed";
        case Client_Validation_Failed:
            return os << "Client_Validation_Failed";
        case To_Venue_Routing_Failed:
            return os << "To_Venue_Routing_Failed";
        case To_Venue_Validation_Failed:
            return os << "To_Venue_Validation_Failed";
        case To_Venue_Enrichement_Failed:
            return os << "To_Venue_Enrichement_Failed";
        case To_Venue_Encoding_Failed:
            return os << "To_Venue_Encoding_Failed";
        case From_Venue_Decoding_Failed:
            return os << "From_Venue_Decoding_Failed";
        case To_Client_Routing_Failed:
            return os << "To_Client_Routing_Failed";

        default:
            return os << "Unknown";
        }
    }

    class RejectInfo
    {
    public:
        constexpr RejectInfo() = default;
        constexpr RejectInfo(const char *reason, InteranlRejectCode code = InteranlRejectCode::None) noexcept
            : code_(code), reason_(reason) {}

        constexpr operator bool() const noexcept { return code_ == InteranlRejectCode::None; }

        friend std::ostream &operator<<(std::ostream &os, const RejectInfo &reject_info)
        {
            os << "code:" << reject_info.code_ << " reason:" << reject_info.reason_;
            return os;
        }

    private:
        InteranlRejectCode code_{InteranlRejectCode::None};
        const char *reason_{""};
    };

}