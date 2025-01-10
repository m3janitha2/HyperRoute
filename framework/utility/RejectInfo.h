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
        /* Validatator */
        Validatator_Validation_Failed,
        /* Enricher */
        SourceSession_Enrichment_Failed,
        SourceSession_Validation_Failed,
        DestinationSession_Enrichment_Failed,
        DestinationSession_Validation_Failed,
    };

    const char *to_chars(InteranlRejectCode code) noexcept;
    std::string to_string(InteranlRejectCode code) noexcept;

    using RejectInfo = ErrorInfo<InteranlRejectCode>;
}