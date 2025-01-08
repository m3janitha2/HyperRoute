message("CMAKE_CURRENT_LIST_DIR is ${CMAKE_CURRENT_LIST_DIR}")

include_directories(${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/../framework)

add_subdirectory(session)
add_subdirectory(protocol)
add_subdirectory(validator)
add_subdirectory(trading_system)
