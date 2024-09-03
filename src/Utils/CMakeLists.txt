add_library(Utils STATIC
        Noncopyable.hpp
        DiskSpaceHelper.hpp
        ThreadPool.hpp
        Logger.hpp
        Path.cpp
        Path.h)

target_link_libraries(Utils Qt${QT_VERSION_MAJOR}::Core)

target_include_directories(Utils PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})