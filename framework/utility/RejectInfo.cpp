#include <framework/utility/RejectInfo.h>

namespace hyper::framework
{
    constexpr const char *to_chars(InteranlRejectCode code) noexcept
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

    std::string to_string(InteranlRejectCode code) noexcept
    {
        return to_chars(code);
    }

}