message("CMAKE_CURRENT_LIST_DIR is ${CMAKE_CURRENT_LIST_DIR}")

include_directories(${PROJECT_SOURCE_DIR})

add_library(transport OBJECT)

file(GLOB transport_sources
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
)

target_sources(transport PRIVATE ${transport_sources})

target_include_directories(transport PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
