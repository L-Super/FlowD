find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Sql)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Sql REQUIRED)

include(FetchContent)
FetchContent_Declare(
        Catch2
        URL https://github.com/catchorg/Catch2/archive/refs/tags/v3.6.0.tar.gz
        GIT_TAG 4e8d92b #v3.6.0
)
FetchContent_MakeAvailable(Catch2)

add_executable(DownloadTaskTest DownloadTaskTest.cpp)
target_link_libraries(DownloadTaskTest PRIVATE Catch2::Catch2WithMain)

add_test(NAME DownloadTaskTest
        COMMAND $<TARGET_FILE:DownloadTaskTest> --success
)

add_executable(DownloadHistoryTest DownloadHistoryTest.cpp)
target_link_libraries(DownloadHistoryTest PRIVATE Catch2::Catch2WithMain Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Sql)

add_test(NAME DownloadHistoryTest
        COMMAND $<TARGET_FILE:DownloadHistoryTest> --success
)