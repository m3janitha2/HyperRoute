#include <iostream>
#include <stdexcept>
#include <string>
#include <random>
#include <memory>
#include <vector>
#include <string_view>
#include <framework/simulator/TCPSimulator.h>
#include <examples/trading_system/application/TestMessageStore.h>

using namespace hyper;

struct SimulatorClientA
{
	SimulatorClientA(const Configuration &config, const std::string &name, std::size_t number_of_messages)
		: sim{config, name,
			  [this](std::string_view data) noexcept
			  { return on_data(data); }},
		  number_of_messages_(number_of_messages),
		  msg_store_(number_of_messages) {}

	std::size_t on_data(std::string_view data) noexcept
	{
		std::cout << "Received:" << data << std::endl;
		return data.length();
	}

	void run()
	{
		std::cout << "number_of_messages: " << number_of_messages_ << std::endl;
		auto sender_thread = std::jthread([this]()
										 {
											static std::uint64_t cl_ord_id{1000};
											for (std::size_t i = 0; i < number_of_messages_; ++i)
											{
												std::cout << "sending: " << cl_ord_id << std::endl; 
												std::this_thread::sleep_for(std::chrono::microseconds(10));
												if(auto ret = sim.send_data_async(msg_store_.get_new_order(cl_ord_id++)); 
													ret != true)
													{
														std::cerr << "Failed to send: " << cl_ord_id << std::endl;
													}
											} });

		sim.run();
		sender_thread.join();

		if (auto ret = sim.disconnect(); ret != true)
			std::cout << ret << std::endl;
	}

	framework::TCPSimulator sim;
	std::size_t number_of_messages_;
	TestMessageStore msg_store_;
};

int main(int argc, char **argv)
{
	if (argc < 8)
	{
		std::cerr << "Usage: ./tcp_test <local_ip> <local_port> <remote_ip> <remote_port> <socket_type> <socket_role> <name> <number_of_messages>\n";
		return 1;
	}

	std::string local_ip = argv[1];
	std::uint16_t local_port = std::stoul(argv[2]);
	std::string remote_ip = argv[3];
	std::uint16_t remote_port = std::stoul(argv[4]);
	std::string socket_type = argv[5];
	std::string socket_role = argv[6];
	std::string name = argv[7];
	std::size_t number_of_messages = std::stoul(argv[8]);

	const auto config = framework::TCPSimulator::generate_config(local_ip, local_port,
																 remote_ip, remote_port,
																 socket_type, socket_role);

	SimulatorClientA sim{config, name, number_of_messages};
	sim.run();
	return 0;
}