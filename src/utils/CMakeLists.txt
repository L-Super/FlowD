add_library(Utils OBJECT
        Noncopyable.hpp
        DiskSpaceHelper.hpp
        ThreadPool.hpp
        Logger.hpp
        Path.cpp Path.h
        AppConfig.cpp AppConfig.h
        LanguageCode.hpp
        Utils.cpp Utils.h
)

target_link_libraries(Utils Qt${QT_VERSION_MAJOR}::Core spdlog::spdlog_header_only nlohmann_json::nlohmann_json)

if (WIN32)

elseif (APPLE)
    target_sources(Utils PRIVATE MacSystemTheme.mm MacSystemTheme.h)
    target_link_libraries(Utils "-framework Cocoa")
elseif (UNIX AND NOT APPLE)
    # enable it in the future for system theme check
#    if(${QT_VERSION_MAJOR} EQUAL 5)
#        find_package(PkgConfig REQUIRED)
#        pkg_check_modules(GTK2 REQUIRED "gtk+-2.0")
#        if(DEFINED GTK2_INCLUDE_DIRS)
#            target_include_directories(Utils PUBLIC ${GTK2_INCLUDE_DIRS})
#        endif()
#        if(DEFINED GTK2_LIBRARY_DIRS)
#            target_link_directories(Utils PUBLIC ${GTK2_LIBRARY_DIRS})
#        endif()
#        if(DEFINED GTK2_CFLAGS_OTHER)
#            target_compile_options(Utils PUBLIC ${GTK2_CFLAGS_OTHER})
#        endif()
#        if(DEFINED GTK2_LIBRARIES)
#            target_link_libraries(Utils PUBLIC ${GTK2_LIBRARIES})
#        endif()
#    endif()
#
#    if(${QT_VERSION_MAJOR} EQUAL 6)
#        find_package(PkgConfig REQUIRED)
#        pkg_check_modules(GTK3 REQUIRED "gtk+-3.0")
#        if(DEFINED GTK3_INCLUDE_DIRS)
#            target_include_directories(Utils PUBLIC ${GTK3_INCLUDE_DIRS})
#        endif()
#        if(DEFINED GTK3_LIBRARY_DIRS)
#            target_link_directories(Utils PUBLIC ${GTK3_LIBRARY_DIRS})
#        endif()
#        if(DEFINED GTK3_CFLAGS_OTHER)
#            target_compile_options(Utils PUBLIC ${GTK3_CFLAGS_OTHER})
#        endif()
#        if(DEFINED GTK3_LIBRARIES)
#            target_link_libraries(Utils PUBLIC ${GTK3_LIBRARIES})
#        endif()
#    endif()
endif ()

target_include_directories(Utils PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})