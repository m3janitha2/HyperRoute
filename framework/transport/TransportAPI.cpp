#include <framework/transport/TransportAPI.h>

namespace hyper::framework
{
    RejectInfo TransportAPI::send_data(std::string_view data) noexcept
    {
        receive_data_cb_for_test_(data);
        return RejectInfo{};
    }

    std::size_t TransportAPI::on_data(std::string_view data, Timestamp timestamp) noexcept
    {
        return transport_callbacks_.data_callback_(data, timestamp);
    }
    
    void TransportAPI::set_receive_data_cb_for_test(std::function<void(std::string_view)> cb) noexcept
    {
        receive_data_cb_for_test_ = std::move(cb);
    }
}