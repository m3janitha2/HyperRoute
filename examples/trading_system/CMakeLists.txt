message("CMAKE_CURRENT_LIST_DIR is ${CMAKE_CURRENT_LIST_DIR}")

include_directories(${PROJECT_SOURCE_DIR})

# Add subdirectories for modules
add_subdirectory(protocol)
add_subdirectory(session)
add_subdirectory(application)
