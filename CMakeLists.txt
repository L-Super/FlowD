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

# download CPM.cmake
set(CPM_VERSION 0.42.0)
set(CPM_LOCATION "${CMAKE_BINARY_DIR}/cmake/CPM_${CPM_VERSION}.cmake")

if(NOT (EXISTS ${CPM_LOCATION}))
    message(STATUS "Downloading CPM.cmake")
    file(DOWNLOAD https://github.com/TheLartians/CPM.cmake/releases/download/v${CPM_VERSION}/CPM.cmake ${CPM_LOCATION})
endif()

include(${CPM_LOCATION})

add_subdirectory(src)

if (BUILD_TEST)
    message(STATUS "Building tests")
    enable_testing()
    add_subdirectory(tests)
endif ()
