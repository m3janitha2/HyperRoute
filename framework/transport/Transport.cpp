#include <framework/transport/Transport.h>

namespace hyper::framework
{
    RejectInfo Transport::send_data(std::string_view data) noexcept
    {
        receive_data_cb_for_test_(data);
        return RejectInfo{};
    }

    void Transport::set_receive_data_cb_for_test(const std::function<void(std::string_view data)> &cb) noexcept
    {
        receive_data_cb_for_test_ = cb;
    }
}