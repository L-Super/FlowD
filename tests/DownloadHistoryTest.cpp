#include <catch2/catch_test_macros.hpp>

#include "../src/logic/DownloadHistory.h"

TEST_CASE("history", "[test1]")
{
    SECTION("insert task")
    {
        bool result = DownloadHistory::instance().insertDownloadTask("https://d/file.txt", "file.txt",
                                                                     "~/home/file.txt", 1024, 8);
        REQUIRE(result == true);
        result = DownloadHistory::instance().insertDownloadTask("https://d2/file.txt", "file2.txt", "~/home/file2.txt",
                                                                20480, 16);
        REQUIRE(result == true);
    }
    SECTION("insert repeat task")
    {
        bool result = DownloadHistory::instance().insertDownloadTask("https://d/file.txt", "file.txt",
                                                                     "~/home/file.txt", 1024, 8);
        REQUIRE(result == false);
    }
    SECTION("insert segment")
    {
        bool result = DownloadHistory::instance().insertDownloadSegment("https://d/file.txt", 0, 1024, 10);
        REQUIRE(result == true);
        result = DownloadHistory::instance().insertDownloadSegment("https://d/file.txt", 1025, 2048, 0);
        REQUIRE(result == true);
    }
    SECTION("get complete task count")
    {
        auto result = DownloadHistory::instance().getCompleteDownloadTasks();
        REQUIRE(result.size() == 0);
    }
    SECTION("get incomplete task count")
    {
        auto result = DownloadHistory::instance().getIncompleteDownloadTasks();
        REQUIRE(result.size() == 2);
    }
    SECTION("update segment")
    {
        bool result = DownloadHistory::instance().updateSegmentProgress("https://d/file.txt", 0, 1024, true);
        REQUIRE(result == true);
    }
}