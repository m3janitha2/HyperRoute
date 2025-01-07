cmake_minimum_required (VERSION 3.16)

project(HyperRoute)

set(CMAKE_CPP_COMPILER g++)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS NO)
set(CMAKE_VERBOSE_MAKEFILE ON)

include_directories(/usr/include)

# set(CMAKE_CONFIGURATION_TYPES "Release" CACHE STRING "" FORCE)

# if (MSVC)
#     # warning level 4 and all warnings as errors
#     add_compile_options(/W4 /WX)
# else()
#     # lots of warnings and all warnings as errors
#     message("setting CMAKE_CXX_FLAGS")
#     set(CMAKE_BUILD_TYPE Release)
#     set(CMAKE_CXX_FLAGS "-O3")
# endif()

include(FetchContent)
FetchContent_Declare(
	googletest
	GIT_REPOSITORY https://github.com/google/googletest.git
	GIT_TAG release-1.12.1
)

FetchContent_MakeAvailable(googletest)

#add_subdirectory(app)
add_subdirectory(application)
add_subdirectory(framework)

# this helps remove the MSVCRTD warning
#add_definitions(-DNODEFAULTLIB:MSVCRTD)

# this helps remove a bunch of static vs dynamic erros
#set(CMAKE_CXX_FLAGS_DEBUG "/MTd")

#add_executable(max_test 
#	src/test.cpp src/Application.h src/Application.cpp src/Messages.h src/NewsTracker.cpp src/NewsTracker.h)

#target_link_libraries(max_test GTest::gtest GTest::gtest_main)
#add_test(NAME max_test COMMAND max_test)



