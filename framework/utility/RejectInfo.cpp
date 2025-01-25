#include <framework/utility/RejectInfo.h>

namespace hyper::framework
{
    const char *to_chars(InteranlRejectCode code) noexcept
    {
        using enum InteranlRejectCode;
        switch (code)
        {
        case SourceSession_Invalid_Original_Msg_ID:
            return "SourceSession_Invalid_Original_Msg_ID";
        case DestinationRouter_No_Destinations_Available:
            return "DestinationRouter_No_Destinations_Available";
        case DestinationRouter_Session_Not_Found_For_UID:
            return "DestinationRouter_Session_Not_Found_For_UID";
        case SourceRouter_Routing_Failed:
            return "SourceRouter_Routing_Failed";
        case DestinationSession_To_Destination_Encoding_Failed:
            return "DestinationSession_To_Destination_Encoding_Failed";
        case DestinationSession_From_Destination_Decoding_Failed:
            return "DestinationSession_From_Destination_Decoding_Failed";
        case Validatator_Validation_Failed:
            return "Validatator_Validation_Failed";
        case SourceSession_Transform_Failed:
            return "SourceSession_Transform_Failed";
        case SourceSession_Validation_Failed:
            return "SourceSession_Validation_Failed";
        case DestinationSession_Transform_Failed:
            return "DestinationSession_Transform_Failed";
        case DestinationSession_Validation_Failed:
            return "DestinationSession_Validation_Failed";
        default:
            return "Unknown_Reject_Code";
        }
    }

    std::string to_string(InteranlRejectCode code) noexcept
    {
        return to_chars(code);
    }

}