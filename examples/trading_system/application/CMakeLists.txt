message("CMAKE_CURRENT_LIST_DIR is ${CMAKE_CURRENT_LIST_DIR}")

include_directories(${PROJECT_SOURCE_DIR})
message("include_directories: ${PROJECT_SOURCE_DIR}")

set(SOURCE_FILES
    main.cpp
    ../session/SourceSessionProtocolA.cpp
    ../protocol/ProtocolA.cpp
    ../../../framework/config/ConfigManager.cpp
    ../../../framework/router/SourceRouter.cpp
    ../../../framework/utility/RejectInfo.cpp
    ../../../framework/transport/Transport.cpp
    ../session/DestinationSessionProtocolB.cpp
    ../protocol/ProtocolB.cpp
    ../../../framework/application/application.cpp)

add_executable(trading_system
    ${SOURCE_FILES})

target_compile_options(trading_system PRIVATE -Wall -Wextra -Wpedantic -fconcepts-diagnostics-depth=2)
