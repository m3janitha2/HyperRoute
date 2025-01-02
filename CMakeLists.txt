cmake_minimum_required (VERSION 3.16)

project(max)

set(CMAKE_CPP_COMPILER g++)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS NO)
set(CMAKE_VERBOSE_MAKEFILE ON)

include_directories(/usr/include)

include(FetchContent)
FetchContent_Declare(
	googletest
	GIT_REPOSITORY https://github.com/google/googletest.git
	GIT_TAG release-1.12.1
)

FetchContent_MakeAvailable(googletest)

#add_subdirectory(app)
add_subdirectory(application)

# this helps remove the MSVCRTD warning
#add_definitions(-DNODEFAULTLIB:MSVCRTD)

# this helps remove a bunch of static vs dynamic erros
#set(CMAKE_CXX_FLAGS_DEBUG "/MTd")

#add_executable(max_test 
#	src/test.cpp src/Application.h src/Application.cpp src/Messages.h src/NewsTracker.cpp src/NewsTracker.h)

#target_link_libraries(max_test GTest::gtest GTest::gtest_main)
#add_test(NAME max_test COMMAND max_test)



