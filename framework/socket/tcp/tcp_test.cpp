#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <framework/socket/tcp/EpollSocketManager.h>
#include <framework/simulator/TCPSimulator.h>
#include <framework/config/Configuration.h>

using namespace hyper::framework;

int main(int argc, char **argv)
{
  if (argc < 8)
  {
    std::cerr << "Usage: ./a <string> <int1> <int2>\n";
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

  Configuration cfg;
  cfg.put("local_ip", local_ip);
  cfg.put("local_port", local_port);
  cfg.put("remote_ip", remote_ip);
  cfg.put("remote_port", remote_port);
  cfg.put("socket_type", socket_type);
  cfg.put("socket_role", socket_role);

  TCPSimulator sim{name, cfg};
  if (auto ret = sim.connect(); ret != true)
  {
    std::cout << ret << std::endl;
  }
  sim.send_data(message);
  sim.run();
  if (auto ret = sim.disconnect(); ret != true)
  {
    std::cout << ret << std::endl;
  }

  return 0;
}