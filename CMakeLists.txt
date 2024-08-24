cmake_minimum_required(VERSION 3.26)

project(QtProjectTemplate
        VERSION 1.0.0.0 # Set application version. For more details, see src/version.h.in
)

set(CMAKE_CXX_STANDARD 17)

option(BUILD_TEST "Build the test" OFF)

message(STATUS "CMake build type is ${CMAKE_BUILD_TYPE}")
message(STATUS "The application's version is ${CMAKE_PROJECT_VERSION}")

add_subdirectory(src)

if (BUILD_TEST)
    enable_testing()
    add_subdirectory(tests)
endif ()