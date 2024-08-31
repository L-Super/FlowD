add_library(Utils
        Noncopyable.hpp
        DiskSpaceHelper.hpp)
target_sources(${PROJECT_NAME} PUBLIC
            Logger.hpp
 )
target_include_directories(Utils PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
