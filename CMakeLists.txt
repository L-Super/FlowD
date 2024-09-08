cmake_minimum_required(VERSION 3.26)

project(FLowD
        VERSION 1.0.0.0 # Set application version. For more details, see src/version.h.in
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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

include(CheckIPOSupported)
check_ipo_supported(RESULT IPO_SUPPORTED)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ${IPO_SUPPORTED})

add_subdirectory(src)

if (BUILD_TEST)
    message(STATUS "Building tests")
    enable_testing()
    add_subdirectory(tests)
endif ()