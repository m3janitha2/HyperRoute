#pragma once

#include <framework/utility/ErrorInfo.h>

namespace hyper::framework
{
    enum class InteranlRejectCode
    {
        Success,
        /* SourceSession */
        SourceSession_Invalid_Original_Msg_ID,
        /* DestinationRouter */
        DestinationRouter_No_Destinations_Available,
        DestinationRouter_Session_Not_Found_For_UID,
        /* SourceRouter */
        SourceRouter_Routing_Failed,
        /* DestinationSession */
        DestinationSession_To_Destination_Encoding_Failed,
        DestinationSession_From_Destination_Decoding_Failed,
        DestinationSession_Invalid_Original_Msg_ID,
        DestinationSession_Source_Msg_ID_Not_Found,
        /* Validatator */
        Validatator_Validation_Failed,
        /* Transform */
        SourceSession_Transform_Failed,
        SourceSession_Validation_Failed,
        DestinationSession_Transform_Failed,
        DestinationSession_Validation_Failed,
    };

    const char *to_chars(InteranlRejectCode code) noexcept;
    std::string to_string(InteranlRejectCode code) noexcept;

    using RejectInfo = ErrorInfo<InteranlRejectCode>;
}