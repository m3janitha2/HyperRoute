# Print the current directory for debugging
message("CMAKE_CURRENT_LIST_DIR is ${CMAKE_CURRENT_LIST_DIR}")

# Include the root project's source directory for headers
include_directories(${PROJECT_SOURCE_DIR})
message("Include directories: ${PROJECT_SOURCE_DIR}")

# Collect all sources for trading_system
file(GLOB app_benchmark_core_sources
    "${CMAKE_CURRENT_LIST_DIR}/app_benchmark_core_sources.cpp"
    # SourceRouter depends on example message types, needs refactoring
    "${CMAKE_SOURCE_DIR}/framework/router/SourceRouter.cpp"
)

# Create the executable for trading_system
add_executable(app_benchmark_core ${app_benchmark_core_sources})

# Link object libraries (e.g., ts_session, ts_protocol)
# Ensure these object libraries are defined in their respective subdirectories
target_sources(app_benchmark_core PRIVATE
    $<TARGET_OBJECTS:ts_session>
    $<TARGET_OBJECTS:ts_protocol>
)

# Link the framework library
target_link_libraries(app_benchmark_core PRIVATE framework)

# Include framework headers
target_include_directories(app_benchmark_core PRIVATE
    ${CMAKE_SOURCE_DIR}/framework
)

# Add compile options for stricter checks
target_compile_options(app_benchmark_core PRIVATE
    -Wall # Enable all warnings
    -Wextra # Enable extra warnings
    -Wpedantic # Enforce strict language conformance
    -fconcepts-diagnostics-depth=2 # Diagnostic depth for C++20 concepts
    -Werror # Treat warnings as errors
)

file(GLOB app_trading_system_sources
    "${CMAKE_CURRENT_LIST_DIR}/application.cpp"
    # SourceRouter depends on example message types, needs refactoring
    "${CMAKE_SOURCE_DIR}/framework/router/SourceRouter.cpp"
)

# Create the executable for trading_system
add_executable(app_trading_system ${app_trading_system_sources})

# Link object libraries (e.g., ts_session, ts_protocol)
# Ensure these object libraries are defined in their respective subdirectories
target_sources(app_trading_system PRIVATE
    $<TARGET_OBJECTS:ts_session>
    $<TARGET_OBJECTS:ts_protocol>
)

# Link the framework library
target_link_libraries(app_trading_system PRIVATE framework)

# Include framework headers
target_include_directories(app_trading_system PRIVATE
    ${CMAKE_SOURCE_DIR}/framework
)

# Add compile options for stricter checks
target_compile_options(app_trading_system PRIVATE
    -Wall # Enable all warnings
    -Wextra # Enable extra warnings
    -Wpedantic # Enforce strict language conformance
    -fconcepts-diagnostics-depth=2 # Diagnostic depth for C++20 concepts
    -Werror # Treat warnings as errors
)

##################################################################################

file(GLOB sim_client_a_sources
    "${CMAKE_CURRENT_LIST_DIR}/sim_client_a.cpp"
    # SourceRouter depends on example message types, needs refactoring
    "${CMAKE_SOURCE_DIR}/framework/router/SourceRouter.cpp"
)

# Create the executable for trading_system
add_executable(sim_client_a ${sim_client_a_sources})

# Link object libraries (e.g., ts_session, ts_protocol)
# Ensure these object libraries are defined in their respective subdirectories
target_sources(sim_client_a PRIVATE
    $<TARGET_OBJECTS:ts_session>
    $<TARGET_OBJECTS:ts_protocol>
)

# Link the framework library
target_link_libraries(sim_client_a PRIVATE framework)

# Include framework headers
target_include_directories(sim_client_a PRIVATE
    ${CMAKE_SOURCE_DIR}/framework
)

# Add compile options for stricter checks
target_compile_options(sim_client_a PRIVATE
    -Wall # Enable all warnings
    -Wextra # Enable extra warnings
    -Wpedantic # Enforce strict language conformance
    -fconcepts-diagnostics-depth=2 # Diagnostic depth for C++20 concepts
    -Werror # Treat warnings as errors
)

##################################################################################

file(GLOB sim_server_b_sources
    "${CMAKE_CURRENT_LIST_DIR}/sim_server_b.cpp"
    # SourceRouter depends on example message types, needs refactoring
    "${CMAKE_SOURCE_DIR}/framework/router/SourceRouter.cpp"
)

# Create the executable for trading_system
add_executable(sim_server_b ${sim_server_b_sources})

# Link object libraries (e.g., ts_session, ts_protocol)
# Ensure these object libraries are defined in their respective subdirectories
target_sources(sim_server_b PRIVATE
    $<TARGET_OBJECTS:ts_session>
    $<TARGET_OBJECTS:ts_protocol>
)

# Link the framework library
target_link_libraries(sim_server_b PRIVATE framework)

# Include framework headers
target_include_directories(sim_server_b PRIVATE
    ${CMAKE_SOURCE_DIR}/framework
)

# Add compile options for stricter checks
target_compile_options(sim_server_b PRIVATE
    -Wall # Enable all warnings
    -Wextra # Enable extra warnings
    -Wpedantic # Enforce strict language conformance
    -fconcepts-diagnostics-depth=2 # Diagnostic depth for C++20 concepts
    -Werror # Treat warnings as errors
)