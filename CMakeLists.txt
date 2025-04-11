cmake_minimum_required(VERSION 3.26)

project(FLowD
        VERSION 1.0.0.0 # Set application version. For more details, see src/version.h.in
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif ()

option(BUILD_TEST "Build the test" OFF)

message(STATUS "CMake build type is ${CMAKE_BUILD_TYPE}")
message(STATUS "The application's version is ${CMAKE_PROJECT_VERSION}")

if(WIN32)
    add_compile_definitions(WINDOWS_OS)
elseif(APPLE)
    add_compile_definitions(MAC_OS)
elseif(UNIX AND NOT APPLE)
    add_compile_definitions(LINUX_OS)
endif()

if (MSVC)
    add_compile_options(/W4 /sdl /GS /analyze)
else()
    add_compile_options(-Wall -Wextra -Wnon-virtual-dtor -Wconversion -Wsign-conversion)
endif()

add_subdirectory(src)

if (BUILD_TEST)
    message(STATUS "Building tests")
    enable_testing()
    add_subdirectory(tests)
endif ()
