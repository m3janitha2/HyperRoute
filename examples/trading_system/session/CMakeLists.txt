message("CMAKE_CURRENT_LIST_DIR is ${CMAKE_CURRENT_LIST_DIR}")

include_directories(${PROJECT_SOURCE_DIR})

add_library(ts_session OBJECT)

file(GLOB ts_session_sources "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
target_sources(ts_session PRIVATE ${ts_session_sources})

target_include_directories(ts_session PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})