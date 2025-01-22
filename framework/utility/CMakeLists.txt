message("CMAKE_CURRENT_LIST_DIR is ${CMAKE_CURRENT_LIST_DIR}")

include_directories(${PROJECT_SOURCE_DIR})

add_library(utility OBJECT)

file(GLOB utility_sources "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
target_sources(utility PRIVATE ${utility_sources})

target_include_directories(utility PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
