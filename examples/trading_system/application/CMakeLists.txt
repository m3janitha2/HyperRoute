message("CMAKE_CURRENT_LIST_DIR is ${CMAKE_CURRENT_LIST_DIR}")

include_directories(${PROJECT_SOURCE_DIR})
message("include_directories: ${PROJECT_SOURCE_DIR}")

file(GLOB trading_system_sources
    main.cpp
    # ../session/SourceSessionProtocolA.cpp
    # ../protocol/ProtocolA.cpp
    # ../../../framework/config/ConfigManager.cpp
    ../../../framework/router/SourceRouter.cpp
    # ../../../framework/utility/RejectInfo.cpp
    # ../../../framework/transport/Transport.cpp
    # ../../../framework/application/Application.cpp
    # ../session/DestinationSessionProtocolB.cpp
    #../protocol/ProtocolB.cpp
    )

add_executable(trading_system ${trading_system_sources})

# Link object libraries
target_sources(trading_system PRIVATE
    $<TARGET_OBJECTS:ts_session>
    $<TARGET_OBJECTS:ts_protocol>
)

# Link the framework library
target_link_libraries(trading_system PRIVATE framework)

# Include framework headers
target_include_directories(trading_system PRIVATE
    ${CMAKE_SOURCE_DIR}/framework
)

target_compile_options(trading_system PRIVATE -Wall -Wextra -Wpedantic -fconcepts-diagnostics-depth=2)
