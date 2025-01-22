message("CMAKE_CURRENT_LIST_DIR is ${CMAKE_CURRENT_LIST_DIR}")

include_directories(${PROJECT_SOURCE_DIR})

add_subdirectory(trading_system)

# Add compile options for stricter checks
add_compile_options(
    -Wall # Enable all warnings
    -Wextra # Enable extra warnings
    -Wpedantic # Enforce strict language conformance
    -fconcepts-diagnostics-depth=2 # Diagnostic depth for C++20 concepts
    -Werror # Treat warnings as errors
)
