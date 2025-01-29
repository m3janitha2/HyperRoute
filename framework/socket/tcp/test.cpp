#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include "EpollSocketManager.h"
#include <framework/simulator/TcpSimulator.h>

using namespace hyper::framework;

int main()
{
    const std::string server_ip = "127.0.0.1";
    const int port = 8081;

    TCPSimulator server_sim{"server1", server_ip, port, TCPSimulator::Type::Server};
    TCPSimulator client_sim{"client1", server_ip, port, TCPSimulator::Type::Client};
    
    std::thread server_thread([&]()
                              { server_sim.run(); });
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    std::thread client_thread([&]()
                              { 
                                std::this_thread::sleep_for(std::chrono::seconds(1));
                                std::string_view data{"Test messsages sending from client"};
                                client_sim.send_data(data);
                                client_sim.run(); });

    server_thread.join();
    client_thread.join();

    return 0;
}