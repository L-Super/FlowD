add_library(Utils OBJECT
        Noncopyable.hpp
        DiskSpaceHelper.hpp
        ThreadPool.hpp
        Logger.hpp
        Path.cpp Path.h
        AppConfig.cpp AppConfig.h
)

target_link_libraries(Utils Qt${QT_VERSION_MAJOR}::Core spdlog::spdlog_header_only nlohmann_json::nlohmann_json)

target_include_directories(Utils PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})