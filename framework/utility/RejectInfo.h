#pragma once

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
        Destination_Is_Not_Connected,
        Destination_Orig_Cl_Ord_Id_Not_Found,
        Source_Cl_ord_Id_Not_Found,
        Validation_Failed_Check_01,
    };    
    
    const char *to_chars(InteranlRejectCode code) noexcept;
    std::string to_string(InteranlRejectCode code) noexcept;

    using RejectInfo = ErrorInfo<InteranlRejectCode>;
}