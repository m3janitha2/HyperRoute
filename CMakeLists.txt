cmake_minimum_required(VERSION 3.16)

project(HyperRoute)

set(CMAKE_CPP_COMPILER g++)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS NO)
set(CMAKE_VERBOSE_MAKEFILE ON)

include_directories(/usr/include)

# Set default to Debug if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Build type" FORCE)
endif()

# set(CMAKE_CONFIGURATION_TYPES "Release" CACHE STRING "" FORCE)

if(MSVC)
    # warning level 4 and all warnings as errors
    add_compile_options(/W4 /WX)
    # this helps remove the MSVCRTD warning
    add_definitions(-DNODEFAULTLIB:MSVCRTD)
else()
    # lots of warnings and all warnings as errors
    message("setting CMAKE_CXX_FLAGS")
    # Debug
    set(CMAKE_CXX_FLAGS_DEBUG "-g")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
endif()

include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.12.1
)

FetchContent_MakeAvailable(googletest)

add_subdirectory(example)
add_subdirectory(framework)



# this helps remove a bunch of static vs dynamic erros
#set(CMAKE_CXX_FLAGS_DEBUG "/MTd")
