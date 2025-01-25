// #include <iostream>
// #include <unordered_map>
// #include <string>
// #include <string_view>
// #include <functional>
// #include <stdexcept>
// #include <functional>
// #include <sys/epoll.h>
// #include <sys/socket.h>
// #include <arpa/inet.h>
// #include <unistd.h>
// #include <fcntl.h>
// #include <mutex>
// #include <queue>
// #include <thread>
// #include <vector>

// namespace hyper::framework::testx
// {
//     struct TransportCallbacks
//     {
//         explicit TransportCallbacks(std::function<void()> connect_callback,
//                                     std::function<void()> disconnect_callback,
//                                     std::function<std::size_t(std::string_view)> data_callback)
//             : connect_callback_{std::move(connect_callback)},
//               disconnect_callback_{std::move(disconnect_callback)},
//               data_callback_{std::move(data_callback)}
//         {
//             if (!connect_callback_ || !disconnect_callback_ || !data_callback_)
//             {
//                 throw std::invalid_argument("All transport callbacks must be set and non-null.");
//             }
//         }

//         std::function<void()> connect_callback_;
//         std::function<void()> disconnect_callback_;
//         std::function<std::size_t(std::string_view)> data_callback_;
//     };

//     inline void set_non_blocking(int fd)
//     {
//         int flags = fcntl(fd, F_GETFL, 0);
//         if (flags == -1)
//             throw std::runtime_error("Failed to get socket flags");
//         if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1)
//             throw std::runtime_error("Failed to set socket to non-blocking");
//     }

//     class EpollSocketManager
//     {
//     public:
//         EpollSocketManager()
//         {
//             epoll_fd_ = epoll_create1(0);
//             if (epoll_fd_ == -1)
//                 throw std::runtime_error("Failed to create epoll instance");
//         }

//         ~EpollSocketManager()
//         {
//             for (auto &[fd, _] : connections_)
//             {
//                 close(fd);
//             }
//             close(epoll_fd_);
//         }

//         int create_server_socket(const std::string &ip, int port)
//         {
//             int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
//             if (sock_fd == -1)
//                 throw std::runtime_error("Failed to create server socket");

//             set_non_blocking(sock_fd);

//             sockaddr_in addr{};
//             addr.sin_family = AF_INET;
//             addr.sin_port = htons(port);
//             if (inet_pton(AF_INET, ip.c_str(), &addr.sin_addr) <= 0)
//             {
//                 close(sock_fd);
//                 throw std::runtime_error("Invalid IP address");
//             }

//             if (bind(sock_fd, reinterpret_cast<sockaddr *>(&addr), sizeof(addr)) == -1)
//             {
//                 close(sock_fd);
//                 throw std::runtime_error("Failed to bind server socket");
//             }

//             if (listen(sock_fd, 10) == -1)
//             {
//                 close(sock_fd);
//                 throw std::runtime_error("Failed to listen on server socket");
//             }

//             return sock_fd;
//         }

//         int create_clinet_socket(const std::string &ip, int port)
//         {
//             int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
//             if (sock_fd == -1)
//                 throw std::runtime_error("Failed to create client socket");

//             set_non_blocking(sock_fd);

//             sockaddr_in addr{};
//             addr.sin_family = AF_INET;
//             addr.sin_port = htons(port);
//             if (inet_pton(AF_INET, ip.c_str(), &addr.sin_addr) <= 0)
//             {
//                 close(sock_fd);
//                 throw std::runtime_error("Invalid IP address");
//             }

//             if (::connect(sock_fd, reinterpret_cast<sockaddr *>(&addr), sizeof(addr)) == -1 && errno != EINPROGRESS)
//             {
//                 close(sock_fd);
//                 throw std::runtime_error("Failed to connect client socket");
//             }

//             return sock_fd;
//         }

//         void add_server(const std::string &ip, int port, TransportCallbacks transport_callbacks)
//         {
//             int sock_fd = create_server_socket(ip, port);
//             add_to_epoll(sock_fd, transport_callbacks, true);
//         }

//         void add_client(const std::string &ip, int port, TransportCallbacks transport_callbacks)
//         {
//             auto sock_fd = create_clinet_socket(ip, port);
//             add_to_epoll(sock_fd, transport_callbacks, false);
//         }

//         void send_data(int fd, std::string_view data)
//         {
//             ssize_t bytes_sent = send(fd, data.data(), data.size(), 0);
//             if (bytes_sent == -1 && errno != EAGAIN)
//             {
//                 std::cerr << "Failed to send data on fd: " << fd << "\n";
//                 remove_connection(fd);
//             }
//         }

//         void process_io_events()
//         {
//             constexpr int max_events = 16;
//             epoll_event events[max_events];

//             while (true)
//             {
//                 int n = epoll_wait(epoll_fd_, events, max_events, -1);
//                 if (n == -1) [[unlikely]]
//                 {
//                     if (errno == EINTR) [[likely]]
//                         continue; // Retry on interrupt
//                     throw std::runtime_error("epoll_wait failed");
//                 }

//                 for (int i = 0; i < n; ++i)
//                 {
//                     int fd = events[i].data.fd;
//                     uint32_t event_mask = events[i].events;

//                     if (connections_.count(fd))
//                     {
//                         auto &connection = connections_[fd];

//                         if (event_mask & EPOLLIN)
//                             handle_receive(connection);

//                         if (event_mask & EPOLLOUT)
//                             handle_send(connection);

//                         if (event_mask & (EPOLLERR | EPOLLHUP))
//                         {
//                             std::cerr << "Connection error on fd: " << fd << "\n";
//                             remove_connection(fd);
//                         }
//                     }
//                     else
//                     {
//                         throw std::runtime_error("unknown file descriptor");
//                     }
//                 }

//                 // Process the send queue
//                 process_send_queue();
//             }
//         }

//     private:
//         struct Connection
//         {
//             int fd;
//             bool is_server;
//             TransportCallbacks
//         };

//         int epoll_fd_;
//         std::unordered_map<int, Connection> connections_;
//         std::mutex send_mutex_;
//         std::queue<std::pair<int, std::string>> send_queue_;

//         void add_to_epoll(int fd, DataCallback on_data_received, bool is_server)
//         {
//             epoll_event event{};
//             event.events = EPOLLIN | EPOLLET | EPOLLOUT;
//             event.data.fd = fd;

//             if (epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, fd, &event) == -1)
//             {
//                 close(fd);
//                 throw std::runtime_error("Failed to add socket to epoll");
//             }

//             connections_.emplace(fd, Connection{fd, is_server, on_data_received});
//         }

//         void remove_connection(int fd)
//         {
//             if (connections_.count(fd))
//             {
//                 epoll_ctl(epoll_fd_, EPOLL_CTL_DEL, fd, nullptr);
//                 close(fd);
//                 connections_.erase(fd);
//             }
//         }

//         void handle_receive(Connection &connection)
//         {
//             char buffer[4096];
//             ssize_t bytes_read = read(connection.fd, buffer, sizeof(buffer));
//             if (bytes_read > 0) [[likely]]
//             {
//                 connection.on_data_received(connection.fd, buffer, static_cast<size_t>(bytes_read));
//             }
//             else if (bytes_read == 0 || (bytes_read == -1 && errno != EAGAIN))
//             {
//                 std::cerr << "Connection closed or error on fd: " << connection.fd << "\n";
//                 remove_connection(connection.fd);
//             }
//         }

//         void handle_send(Connection &connection)
//         {
//             // Placeholder: Actual sending logic is triggered from the send queue
//         }

//         void process_send_queue()
//         {
//             std::lock_guard<std::mutex> lock(send_mutex_);
//             while (!send_queue_.empty())
//             {
//                 auto [fd, data] = send_queue_.front();
//                 send_queue_.pop();

//                 ssize_t bytes_sent = send(fd, data.c_str(), data.size(), 0);
//                 if (bytes_sent == -1 && errno != EAGAIN)
//                 {
//                     std::cerr << "Failed to send data on fd: " << fd << "\n";
//                     remove_connection(fd);
//                 }
//             }
//         }
//     };
// }
