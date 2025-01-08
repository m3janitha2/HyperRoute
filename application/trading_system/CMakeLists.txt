message("CMAKE_CURRENT_LIST_DIR is ${CMAKE_CURRENT_LIST_DIR}")

include_directories(${PROJECT_SOURCE_DIR})
message("include_directories: ${PROJECT_SOURCE_DIR}")

set(SOURCE_FILES
    main.cpp
    ../session/SourceSessionProtocolA.cpp
    ../protocol/ProtocolA.cpp
    ../../framework/router/SourceRouter.cpp
    ../../framework/utility/RejectInfo.cpp
    ../session/DestinationSessionProtocolB.cpp
    ../protocol/ProtocolB.cpp)

add_executable(HyperRoute
    ${SOURCE_FILES})
