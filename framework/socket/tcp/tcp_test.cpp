
#include <framework/socket/tcp/EpollSocketManager.h>
#include <framework/simulator/TCPSimulator.h>
#include <framework/config/Configuration.h>
#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <format>

using namespace hyper::framework;

struct TCPTest
{
  TCPTest(const Configuration &config, const std::string &name, const std::string &message)
      : sim{config, name,
            [this](std::string_view data, Timestamp timestamp) noexcept
            { return on_data(data, timestamp); }},
        message_(message) {}

  std::size_t on_data(std::string_view data, Timestamp timestamp) noexcept
  {
    std::cout << "Timestamp:" << timestamp.time_since_epoch().count() << " Received:" << data << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(5));
    if (auto reject_info = sim.send_data(data);
        reject_info != true)
      std::cerr << "Falied to send: " << data << std::endl;
    return data.length();
  }

  void send_data(std::string_view data)
  {
    if (auto reject_info = sim.send_data(data);
        reject_info != true)
      std::cerr << "Falied to send: " << data << std::endl;
  }

  void run()
  {
    // if (!sim.is_server())
    // {
    //   if (auto ret = sim.connect(); ret != true)
    //     std::cout << ret << std::endl;
    // }
    // auto t = std::jthread([&]()
    //                      {
    //   static constexpr const char* message = "async message";
    //   while(true)
    //   {
    //     std::this_thread::sleep_for(std::chrono::seconds(5));
    //     sim.send_data_async(message);
    //   }; });
    send_data(message_);
    sim.run();
    // t.join();
    if (auto ret = sim.disconnect(); ret != true)
      std::cout << ret << std::endl;
  }

  TCPSimulator sim;
  const std::string message_{};
};

int main(int argc, char **argv)
{
  if (argc < 8)
  {
    std::cerr << "Usage: ./tcp_test <local_ip> <local_port> <remote_ip> <remote_port> <socket_type> <socket_role> <name> <message>\n";
    return 1;
  }

  std::string local_ip = argv[1];
  std::uint16_t local_port = std::stoul(argv[2]);
  std::string remote_ip = argv[3];
  std::uint16_t remote_port = std::stoul(argv[4]);
  std::string socket_type = argv[5];
  std::string socket_role = argv[6];
  std::string name = argv[7];
  std::string message = name + " " + argv[8];

  const auto config = TCPSimulator::generate_config(local_ip, local_port,
                                                    remote_ip, remote_port,
                                                    socket_type, socket_role);

  TCPTest test{config, name, message};
  test.run();

  return 0;
}