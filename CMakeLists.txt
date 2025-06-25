cmake_minimum_required(VERSION 3.26)

project(FLowD
        VERSION 1.0.0.0 # Set application version. For more details, see src/version.h.in
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS
        Core
        Gui
        Widgets
        Sql
        LinguistTools
)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS
        Core
        Gui
        Widgets
        Sql
        LinguistTools
        REQUIRED)

#set(CMAKE_AUTOMOC ON)
#set(CMAKE_AUTORCC ON)
#set(CMAKE_AUTOUIC ON)
# require:Qt 6.3. refer:https://doc.qt.io/qt-6/qt-standard-project-setup.html
qt_standard_project_setup(REQUIRES 6.5
        I18N_SOURCE_LANGUAGE en-US
        I18N_TRANSLATED_LANGUAGES zh-CN
)

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

add_subdirectory(src)

if (BUILD_TEST)
    message(STATUS "Building tests")
    enable_testing()
    add_subdirectory(tests)
endif ()
