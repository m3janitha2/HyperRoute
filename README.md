# HyperRoute
**A general-purpose, stateless, ultra-low-latency message router capable of routing messages between source and destination in under 300ns (excluding transport layer overhead). Built for seamless extensibility and customization.**

## Features
1. Fully customizable and plug-and-play source and destination protocols.
2. Flexible and extendable routing strategies.
3. Extendable message transformation and validation.
   
The protocol-agnostic core ensures independence from specific messaging protocols. Protocols, routers, validators, and transforms can be seamlessly integrated to accommodate custom requirements.

## Performance  
Check the [GitHub Actions Benchmark](https://github.com/m3janitha2/HyperRoute/actions) for detailed performance metrics.  

**Please note:** GitHub Actions runners have limited flexibility in terms of hardware and system configurations. The CPU is low-spec, and you cannot:  
- Choose specific hardware  
- Modify kernel parameters such as C-States, P-States, or Spectre/Meltdown patches  
- Isolate CPUs, enable a tickless kernel, or configure interrupt isolation  

Additionally, binding critical threads to CPUs without isolated cores may degrade performance.

## Note
**_This is a work in progress._** If you are interested in new features, feel free to reach out. I am a low-latency enthusiast and open to collaborating or working on similar projects.

## Possible Applications  
1. **Traffic Distribution**  
   Efficiently route requests across multiple servers to optimize resource utilization.  
2. **Fault Tolerance**  
   Ensure high availability by redirecting traffic from failed or overloaded servers.  
3. **Protocol Integration**  
   Connect and transform messages across diverse protocols.  
4. **Content-Based Routing**  
   Route messages based on metadata, such as user requests or geographic location.  
5. **Dynamic Scaling**  
   Support auto-scaling by rerouting traffic to newly added servers in real time.  
6. **Real-Time Data Synchronization**  
   Maintain data consistency by enabling seamless, real-time message routing.  
7. **IoT Data Aggregation**  
   Collect and route IoT data to analytics platforms or cloud storage for insights.

## Design Considerations  
1. **KISS** – "Keep it simple, stupid."  
2. **Static Interfaces in Critical Paths**  
3. **Zero Copy and Zero Allocation in Critical Paths**  
   - If allocation is unavoidable, prefer stack allocation.  
4. **Move Decisions to Compile Time**  
   - Use type-based decisions instead of runtime variable values (basic metaprogramming).  
   - **Tradeoff**: Improved runtime performance at the cost of increased verbosity and compile time.
  
## What's Next
- Network protocol layer in development, featuring a flexible socket layer supporting TCP, UDP, and kernel bypass (e.g., Mellanox, SolarFlare).
- Customizable threading model to be implemented alongside the socket layer.
