message("CMAKE_CURRENT_LIST_DIR is ${CMAKE_CURRENT_LIST_DIR}")

include_directories(${PROJECT_SOURCE_DIR})
message("include_directories: ${PROJECT_SOURCE_DIR}")

set(SOURCE_FILES
    main.cpp
    ../session/SourceSessionProtocolA.cpp
    ../session/ProtocolA.cpp
    ../../framework/router/SourceRouter.cpp
    ../session/DestinationSessionProtocolB.cpp
    ../session/ProtocolB.cpp)

add_executable(max
    ${SOURCE_FILES})
