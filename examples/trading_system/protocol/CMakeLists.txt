message("CMAKE_CURRENT_LIST_DIR is ${CMAKE_CURRENT_LIST_DIR}")

include_directories(${PROJECT_SOURCE_DIR})

add_library(ts_protocol OBJECT)

file(GLOB ts_protocol_sources "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
target_sources(ts_protocol PRIVATE ${ts_protocol_sources})

target_include_directories(ts_protocol PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})