message("CMAKE_CURRENT_LIST_DIR is ${CMAKE_CURRENT_LIST_DIR}")

include_directories(${PROJECT_SOURCE_DIR})
message("include_directories: ${PROJECT_SOURCE_DIR}")

# Declare framework static library
add_library(framework STATIC)

# Set include directories
target_include_directories(framework PUBLIC ${PROJECT_SOURCE_DIR})

# Add subdirectories for framework components
add_subdirectory(config)
add_subdirectory(transport)
add_subdirectory(application)
add_subdirectory(utility)

# Collect all source files from subdirectories
target_sources(framework PRIVATE
    $<TARGET_OBJECTS:config>
    $<TARGET_OBJECTS:transport>
    $<TARGET_OBJECTS:application>
    $<TARGET_OBJECTS:utility>
)

# Link dependencies explicitly
target_link_libraries(framework PUBLIC
    config
    transport
    utility
    application
)

# Add compile options for stricter checks
add_compile_options(
    -Wall # Enable all warnings
    -Wextra # Enable extra warnings
    -Wpedantic # Enforce strict language conformance
    -fconcepts-diagnostics-depth=2 # Diagnostic depth for C++20 concepts
    -Werror # Treat warnings as errors
)
