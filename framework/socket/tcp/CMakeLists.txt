# Print the current directory for debugging
message("CMAKE_CURRENT_LIST_DIR is ${CMAKE_CURRENT_LIST_DIR}")

# Include the root project's source directory for headers
include_directories(${PROJECT_SOURCE_DIR})

add_library(socket OBJECT)

# Collect all sources for tcp_test
file(GLOB socket_sources
    "${CMAKE_CURRENT_LIST_DIR}/*.h"
    "${CMAKE_CURRENT_LIST_DIR}/*.cpp"
)

target_sources(socket PRIVATE ${socket_sources})

target_include_directories(socket PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Create the executable for tcp_test
add_executable(tcp_test ${socket_sources})

target_link_libraries(tcp_test PUBLIC
    transport
    utility
)

# Add compile options for stricter checks
target_compile_options(tcp_test PRIVATE
    -Wall # Enable all warnings
    -Wextra # Enable extra warnings
    -Wpedantic # Enforce strict language conformance
    -fconcepts-diagnostics-depth=2 # Diagnostic depth for C++20 concepts
    -Werror # Treat warnings as errors
)