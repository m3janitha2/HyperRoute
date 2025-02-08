#include <iostream>
#include <stdexcept>
#include <string>
#include <random>
#include <memory>
#include <vector>
#include <string_view>
#include <framework/simulator/TCPSimulator.h>
#include <examples/trading_system/application/TestMessageStore.h>
#include <examples/trading_system/message/protocol_b/Messages.h>

using namespace hyper;
using namespace hyper::protocol_b;

struct SimulatorServerB
{
	SimulatorServerB(const Configuration &config, const std::string &name, std::size_t number_of_messages)
		: sim{config, name,
			  [this](std::string_view data, Timestamp timestamp) noexcept
			  { return on_data(data, timestamp); }},
		  number_of_messages_(number_of_messages),
		  msg_store_(number_of_messages) {}

	template <typename Msg>
	void on_message(Msg &msg)
	{
		std::cout << "Received: " << ++number_of_messages_received_
				  << " msg: " << msg << std::endl;
	}

	std::size_t on_data(std::string_view data, [[maybe_unused]] Timestamp timestamp) noexcept
	{
		auto *const_header = reinterpret_cast<const schema::Header *>(data.data());
		auto *header = const_cast<schema::Header *>(const_header);
		/* data is less than header size, don't consume */
		if (data.length() < sizeof(schema::Header))
		{
			std::cout << "data is less than header size" << std::endl;
			return 0;
		}

		/* data is less than message size, don't consume */
		if (data.length() < header->size)
		{
			std::cout << "data is less than message size" << std::endl;
			return 0;
		}

		switch (header->msg_type)
		{
		case schema::MsgType::Logon:
		{
			auto &msg = *(reinterpret_cast<schema::Logon *>(header));
			on_message(msg);
			break;
		}
		case schema::MsgType::Logout:
		{
			auto &msg = *(reinterpret_cast<schema::Logout *>(header));
			on_message(msg);
			break;
		}
		case schema::MsgType::Heartbeat:
		{
			auto &msg = *(reinterpret_cast<schema::Heartbeat *>(header));
			on_message(msg);
			break;
		}
		case schema::MsgType::NewOrderSingle:
		{
			session::NewOrderSingle msg{data};
			on_message(msg);
			break;
		}
		case schema::MsgType::CancelReplaceRequest:
		{
			session::CancelReplaceRequest msg{data};
			on_message(msg);
			break;
		}
		case schema::MsgType::CancelRequest:
		{
			session::CancelRequest msg{data};
			on_message(msg);
			break;
		}
		default:
			std::cerr << "invalid message. data[" << data << "]" << std::endl;
			break;
		};

		return header->size;
	}

	void run()
	{
		std::cout << "number_of_messages: " << number_of_messages_ << std::endl;
		// auto sender_thread = std::thread([&]()
		// 								 {
		// 									static std::uint64_t cl_ord_id{1000};
		// 									for (std::size_t i = 0; i < number_of_messages_; ++i)
		// 									{
		// 										std::cout << "sending: " << cl_ord_id << std::endl;
		// 										//std::this_thread::sleep_for(std::chrono::seconds(1));
		// 										sim.send_data(msg_store_.get_new_order(cl_ord_id++));
		// 									} });

		sim.run();
		// sender_thread.join();

		if (auto ret = sim.disconnect(); ret != true)
			std::cout << ret << std::endl;
	}

	framework::TCPSimulator sim;
	std::size_t number_of_messages_;
	TestMessageStore msg_store_;
	std::size_t number_of_messages_received_{0};
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

	SimulatorServerB sim{config, name, number_of_messages};
	sim.run();
	return 0;
}