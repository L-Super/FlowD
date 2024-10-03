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