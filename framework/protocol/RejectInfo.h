#pragma once

#include <cstdint>
#include <framework/utility/ErrorInfo.h>

namespace hyper::framework
{
    enum class InteranlRejectCode
    {
        Success,
        Source_Enrichment_Failed,
        Source_Validation_Failed,
        To_Destination_Routing_Failed,
        To_Destination_Validation_Failed,
        To_Destination_Enrichement_Failed,
        To_Destination_Encoding_Failed,
        From_Destination_Decoding_Failed,
        To_Source_Routing_Failed,
        Invalid_Orig_Cl_Ord_ID,
        Destination_Session_Not_Found_For_UID,
        Validation_Failed_Check_01,
    };

    inline constexpr const char *to_chars(InteranlRejectCode code) noexcept
    {
        using enum InteranlRejectCode;
        switch (code)
        {
        case Source_Enrichment_Failed:
            return "Source_Enrichment_Failed";
        case Source_Validation_Failed:
            return "Source_Validation_Failed";
        case To_Destination_Routing_Failed:
            return "To_Destination_Routing_Failed";
        case To_Destination_Validation_Failed:
            return "To_Destination_Validation_Failed";
        case To_Destination_Enrichement_Failed:
            return "To_Destination_Enrichement_Failed";
        case To_Destination_Encoding_Failed:
            return "To_Destination_Encoding_Failed";
        case From_Destination_Decoding_Failed:
            return "From_Destination_Decoding_Failed";
        case To_Source_Routing_Failed:
            return "To_Source_Routing_Failed";
        case Invalid_Orig_Cl_Ord_ID:
            return "Invalid_Orig_Cl_Ord_ID";
        default:
            return "Unknown";
        }
    }

    inline std::string to_string(InteranlRejectCode code)
    {
        return to_chars(code);
    }

    using RejectInfo = ErrorInfo<InteranlRejectCode>;
}