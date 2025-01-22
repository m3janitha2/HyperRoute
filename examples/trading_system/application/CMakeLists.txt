# Print the current directory for debugging
message("CMAKE_CURRENT_LIST_DIR is ${CMAKE_CURRENT_LIST_DIR}")

# Include the root project's source directory for headers
include_directories(${PROJECT_SOURCE_DIR})
message("Include directories: ${PROJECT_SOURCE_DIR}")

# Collect all sources for trading_system
file(GLOB trading_system_sources
    "${CMAKE_CURRENT_LIST_DIR}/main.cpp"
    # SourceRouter depends on example message types, needs refactoring
    "${CMAKE_SOURCE_DIR}/framework/router/SourceRouter.cpp"
)

# Create the executable for trading_system
add_executable(trading_system ${trading_system_sources})

# Link object libraries (e.g., ts_session, ts_protocol)
# Ensure these object libraries are defined in their respective subdirectories
target_sources(trading_system PRIVATE
    $<TARGET_OBJECTS:ts_session>
    $<TARGET_OBJECTS:ts_protocol>
)

# Link the framework library
target_link_libraries(trading_system PRIVATE framework)

# Include framework headers
target_include_directories(trading_system PRIVATE
    ${CMAKE_SOURCE_DIR}/framework
)

# Add compile options for stricter checks
target_compile_options(trading_system PRIVATE
    -Wall # Enable all warnings
    -Wextra # Enable extra warnings
    -Wpedantic # Enforce strict language conformance
    -fconcepts-diagnostics-depth=2 # Diagnostic depth for C++20 concepts
    -Werror # Treat warnings as errors
)