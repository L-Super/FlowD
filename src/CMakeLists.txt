# configure version
configure_file(version.h.in ${CMAKE_CURRENT_SOURCE_DIR}/version.h @ONLY)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS
        Core
        Gui
        Widgets
)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

get_target_property(QMakeExecutable Qt${QT_VERSION_MAJOR}::qmake IMPORTED_LOCATION)
get_filename_component(QtBinDir "${QMakeExecutable}" DIRECTORY)
find_program(QT_DEPLOY_EXECUTABLE windeployqt macdeployqt HINTS "${QtBinDir}")

message(STATUS "Qt deployment tool: ${QT_DEPLOY_EXECUTABLE}")

qt_add_resources(QRC_FILES resources.qrc)

set(SRC_FILES MainWindow.cpp MainWindow.h MainWindow.ui)

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
    set(COMPANY_NAME "Qt")
    set(FILE_DESCRIPTION "Qt template application") # 文件说明
    set(LEGAL_COPYRIGHT "Copyright 2024-2077 The L Company Ltd.")
    set(PRODUCT_NAME "Qt template")

    set(WIN_RES_PATH ${CMAKE_CURRENT_SOURCE_DIR}/resources/win)
    configure_file(${WIN_RES_PATH}/app.rc.in ${WIN_RES_PATH}/app.rc @ONLY)

    set(QRC_FILES ${QRC_FILES} ${WIN_RES_PATH}/app.rc)

elseif (APPLE)
    set(MAC_RES_PATH "${CMAKE_CURRENT_SOURCE_DIR}/resources/mac")
    # It needs to be added to add_executable to display the bundle icon on macOS
    set(QRC_FILES ${QRC_FILES} ${MAC_RES_PATH}/logo.icns)
endif ()

add_executable(${PROJECT_NAME}
        main.cpp
        ${QRC_FILES}
        ${SRC_FILES}
)

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
            MACOSX_BUNDLE_GUI_IDENTIFIER "com.template.example"
            MACOSX_BUNDLE_INFO_STRING "This is a qt application"
            MACOSX_BUNDLE_BUNDLE_VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}"
            MACOSX_BUNDLE_SHORT_VERSION_STRING "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}"
            MACOSX_BUNDLE_COPYRIGHT "Copyright 2024-2077 The L Company Ltd."
            MACOSX_BUNDLE_INFO_PLIST ${MAC_RES_PATH}/Info.plist.in)
endif ()

target_link_libraries(${PROJECT_NAME}
        Qt${QT_VERSION_MAJOR}::Core
        Qt${QT_VERSION_MAJOR}::Gui
        Qt${QT_VERSION_MAJOR}::Widgets
)

if (WIN32)
    add_custom_command(TARGET ${PROJECT_NAME}
            POST_BUILD
            # Deploy the Qt Application
            COMMAND ${QT_DEPLOY_EXECUTABLE}
            # --no-translations
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