include(FetchContent)
FetchContent_Declare(
        Catch2
        URL https://github.com/catchorg/Catch2/archive/refs/tags/v3.6.0.tar.gz
        GIT_TAG 4e8d92b #v3.6.0
)
FetchContent_MakeAvailable(Catch2)

add_executable(UnitTest test.cpp)
target_link_libraries(UnitTest PRIVATE Catch2::Catch2WithMain)

add_test(NAME UnitTest
        COMMAND $<TARGET_FILE:UnitTest> --success
)