message("CMAKE_CURRENT_LIST_DIR is ${CMAKE_CURRENT_LIST_DIR}")

include_directories(${PROJECT_SOURCE_DIR})

add_subdirectory(validator)
add_subdirectory(persistence)
add_subdirectory(router)
add_subdirectory(session)
add_subdirectory(util)
add_subdirectory(app)
add_subdirectory(config)



