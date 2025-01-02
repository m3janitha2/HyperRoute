message("CMAKE_CURRENT_LIST_DIR is ${CMAKE_CURRENT_LIST_DIR}")
message("CMAKE_SOURCE_DIR is ${CMAKE_SOURCE_DIR}/core/util") 

include_directories(${CMAKE_SOURCE_DIR}/core/util)
include_directories(${CMAKE_CURRENT_LIST_DIR})
