message("CMAKE_CURRENT_LIST_DIR is ${CMAKE_CURRENT_LIST_DIR}")


add_library(application OBJECT)

file(GLOB application_sources
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
)

target_sources(application PRIVATE ${application_sources})

# Set the current source directory as an include directory for the 'application' target
target_include_directories(application PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Link with other libraries, ensuring dependencies are propagated
target_link_libraries(application PUBLIC
    config
    transport
    socket
    utility
)
