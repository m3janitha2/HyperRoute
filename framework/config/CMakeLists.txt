message("CMAKE_CURRENT_LIST_DIR is ${CMAKE_CURRENT_LIST_DIR}")

include_directories(${PROJECT_SOURCE_DIR})

add_library(config OBJECT)

file(GLOB config_sources "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
target_sources(config PRIVATE ${config_sources})

target_include_directories(config PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
