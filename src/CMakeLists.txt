# configure version
configure_file(version.h.in ${CMAKE_CURRENT_SOURCE_DIR}/version.h @ONLY)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS
        Core
        Gui
        Widgets
        Sql
)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS
        Core
        Gui
        Widgets
        Sql
        REQUIRED)

get_target_property(QMakeExecutable Qt${QT_VERSION_MAJOR}::qmake IMPORTED_LOCATION)
get_filename_component(QtBinDir "${QMakeExecutable}" DIRECTORY)
find_program(QT_DEPLOY_EXECUTABLE windeployqt macdeployqt HINTS "${QtBinDir}")

message(STATUS "Qt deployment tool: ${QT_DEPLOY_EXECUTABLE}")

qt_add_resources(QRC_FILES resources.qrc)

set(SRC_FILES MainWindow.cpp MainWindow.h MainWindow.ui
    DownloadListWidget.cpp DownloadListWidget.h DownloadListWidget.ui
    DownloadItemWidget.cpp DownloadItemWidget.h DownloadItemWidget.ui
    HelpWidget.cpp HelpWidget.h HelpWidget.ui
    SettingsWidget.cpp SettingsWidget.h SettingsWidget.ui
    SettingsBasicWidget.cpp SettingsBasicWidget.h SettingsBasicWidget.ui
    SettingsAdvancedWidget.cpp SettingsAdvancedWidget.h SettingsAdvancedWidget.ui
    NewDownloadDialog.cpp NewDownloadDialog.h NewDownloadDialog.ui
    FileInformationWidget.h FileInformationWidget.cpp FileInformationWidget.ui
)

set(APP_DESCRIPTION "An Efficient Multi-threaded Download Manager")
set(APP_COPYRIGHT "Copyright 2024-2077 The FlowD Ltd.")
set(QAPPLICATION_CLASS QApplication CACHE STRING "Inheritance class for SingleApplication")

if (WIN32)
    if (MSVC)
        # Set MSVC to use utf-8 encode for building
        add_compile_options(/utf-8)
    endif ()

    # set Windows rc info
    set(PRODUCT_ICON "logo.ico")
    set(FILE_VERSION ${PROJECT_VERSION_MAJOR},${PROJECT_VERSION_MINOR},${PROJECT_VERSION_PATCH},${PROJECT_VERSION_TWEAK}) # 文件版本
    set(FILE_VERSION_STR "${PROJECT_VERSION}")
    set(PRODUCT_VERSION ${PROJECT_VERSION_MAJOR},${PROJECT_VERSION_MINOR},${PROJECT_VERSION_PATCH},${PROJECT_VERSION_TWEAK}) # 产品版本
    set(PRODUCT_VERSION_STR "${PROJECT_VERSION}")
    set(COMPANY_NAME ${PROJECT_NAME})
    set(FILE_DESCRIPTION ${PROJECT_NAME}) # 文件说明
    set(LEGAL_COPYRIGHT ${APP_COPYRIGHT})
    set(PRODUCT_NAME ${PROJECT_NAME})

    set(WIN_RES_PATH ${CMAKE_CURRENT_SOURCE_DIR}/resources/win)
    configure_file(${WIN_RES_PATH}/app.rc.in ${WIN_RES_PATH}/app.rc @ONLY)

    # add manifest to enable UTF-8 and long paths
    # https://learn.microsoft.com/zh-cn/windows/apps/design/globalizing/use-utf8-code-page
    # https://learn.microsoft.com/zh-cn/windows/win32/fileio/maximum-file-path-limitation?tabs=registry
    configure_file(${WIN_RES_PATH}/app.manifest.xml.in ${WIN_RES_PATH}/${PROJECT_NAME}.manifest @ONLY)

    set(QRC_FILES ${QRC_FILES} ${WIN_RES_PATH}/app.rc ${WIN_RES_PATH}/${PROJECT_NAME}.manifest)

elseif (APPLE)
    set(MAC_RES_PATH "${CMAKE_CURRENT_SOURCE_DIR}/resources/mac")
    # It needs to be added to add_executable to display the bundle icon on macOS
    set(QRC_FILES ${QRC_FILES} ${MAC_RES_PATH}/logo.icns)
endif ()

find_package(Qt${QT_VERSION_MAJOR} COMPONENTS LinguistTools)
set(TS_FILES ${CMAKE_CURRENT_SOURCE_DIR}/resources/i18N/FlowD_zh-CN.ts
        ${CMAKE_CURRENT_SOURCE_DIR}/resources/i18N/FlowD_en-US.ts
)
set_source_files_properties(${TS_FILES} PROPERTIES OUTPUT_LOCATION "${CMAKE_CURRENT_LIST_DIR}/resources/i18N")

if (QT_VERSION_MAJOR EQUAL 6)
    qt_create_translation(QM_FILES ${CMAKE_CURRENT_SOURCE_DIR} ${TS_FILES})
    qt_add_translation(QM_FILES ${TS_FILES})
else ()
    qt5_create_translation(QM_FILES ${CMAKE_CURRENT_SOURCE_DIR} ${TS_FILES}
            #        OPTIONS -no-obsolete -locations none
    )
    qt5_add_translation(QM_FILES ${TS_FILES})
endif ()

add_executable(${PROJECT_NAME}
        main.cpp
        ${QM_FILES}
        ${QRC_FILES}
        ${SRC_FILES}
)
add_subdirectory(logic)
add_subdirectory(utils)

if (MINGW)
    target_compile_options(${PROJECT_NAME} PRIVATE "-Wa,-mbig-obj")
endif ()

if (WIN32)
    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        message(STATUS "Running ${CMAKE_BUILD_TYPE} mode")
    else ()
        message(STATUS "Running ${CMAKE_BUILD_TYPE} mode")
        # Remove the terminal display
        set_target_properties(${PROJECT_NAME} PROPERTIES WIN32_EXECUTABLE TRUE)
    endif ()
elseif (APPLE)
    # It needs after add_executable
    set_target_properties(${PROJECT_NAME} PROPERTIES MACOSX_BUNDLE TRUE)

    # Add icon to Mac bundle Resources dir
    set_source_files_properties(${MAC_RES_PATH}/logo.icns PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")

    set_target_properties(${PROJECT_NAME} PROPERTIES
            MACOSX_BUNDLE_BUNDLE_NAME "${PROJECT_NAME}"
            MACOSX_BUNDLE_ICON_FILE "logo.icns"
            MACOSX_BUNDLE_GUI_IDENTIFIER "com.flowd.flowd"
            MACOSX_BUNDLE_INFO_STRING ${APP_DESCRIPTION}
            MACOSX_BUNDLE_BUNDLE_VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}"
            MACOSX_BUNDLE_SHORT_VERSION_STRING "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}"
            MACOSX_BUNDLE_COPYRIGHT ${APP_COPYRIGHT}
            MACOSX_BUNDLE_INFO_PLIST ${MAC_RES_PATH}/Info.plist.in)
endif ()

message(STATUS "CMake binary dir: ${CMAKE_BINARY_DIR}")

if (WIN32) # Install dlls in the same directory as the executable on Windows
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/src)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/src)
endif ()

CPMAddPackage(
        NAME spdlog
        GITHUB_REPOSITORY gabime/spdlog
        VERSION 1.15.2)

CPMAddPackage(
        NAME json
        VERSION 3.11.3
        # The git repo is incredibly large, so we download the archived include directory
        URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
)

CPMAddPackage(
        NAME SingleApplication
        VERSION 3.5.2
        URL https://github.com/itay-grudev/SingleApplication/archive/refs/tags/v3.5.2.tar.gz
)

CPMAddPackage("gh:libcpr/cpr#96cd60913124f2a9a17f9d814f3db11d62ef3ae0")
CPMAddPackage("gh:vimpunk/mio#8b6b7d878c89e81614d05edca7936de41ccdd2da")
CPMAddPackage("gh:Neargye/magic_enum@0.9.7")


set(MIO_INCLUDE_DIR ${mio_SOURCE_DIR}/single_include)
include_directories(${MIO_INCLUDE_DIR})

target_link_libraries(${PROJECT_NAME} PRIVATE
        Qt${QT_VERSION_MAJOR}::Core
        Qt${QT_VERSION_MAJOR}::Gui
        Qt${QT_VERSION_MAJOR}::Widgets
        Qt${QT_VERSION_MAJOR}::Sql
        spdlog::spdlog_header_only
        nlohmann_json::nlohmann_json
        cpr::cpr
        SingleApplication::SingleApplication
        magic_enum::magic_enum
        Utils
)

if (WIN32)
    add_custom_command(TARGET ${PROJECT_NAME}
            POST_BUILD
            # Deploy the Qt Application
            COMMAND ${QT_DEPLOY_EXECUTABLE}
            --no-translations
            "$<TARGET_FILE:${PROJECT_NAME}>"
    )
elseif (APPLE)
    add_custom_command(TARGET ${PROJECT_NAME}
            POST_BUILD
            COMMAND ${QT_DEPLOY_EXECUTABLE}
            "$<TARGET_BUNDLE_DIR:${PROJECT_NAME}>"
            # -dmg
    )
endif ()
