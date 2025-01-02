#include <application/session/VenueSessionProtocolB.h>
#include "VenueSessionProtocolB.h"

namespace max
{
    void VenueSessionProtocolB::on_data()
    {
        protocol_b::ExecutionReport venue_msg{5, 6};
        on_venue_message(venue_msg);
    }

    void VenueSessionProtocolB::on_venue_message(protocol_b::ExecutionReport &msg)
    {
        procoess_message_from_venue(msg);
    }

    void VenueSessionProtocolB::on_venue_message(protocol_b::CancelReject &msg)
    {
        procoess_message_from_venue(msg);
    }
}