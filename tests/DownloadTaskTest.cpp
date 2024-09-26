#include <catch2/catch_test_macros.hpp>
#include <string>
#include <regex>

TEST_CASE("filename", "[test1]")
{
    std::string filename = "file.zip";
    SECTION("one")
    {
        std::string header = R"(attachment; filename="file.zip")";
        std::regex filenameRegex(R"(filename\s*=\s*["']?([^"']*)["']?)");
        std::smatch match;
        if (std::regex_search(header, match, filenameRegex)) {
            if (match.size() > 1) {
                std::string name = match[1].str();
                REQUIRE(name == filename);
            }
        }
    }
    SECTION("two")
    {
        std::string header = R"(attachment; filename='file.zip')";
        std::regex filenameRegex(R"(filename\s*=\s*["']?([^"']*)["']?)");
        std::smatch match;
        if (std::regex_search(header, match, filenameRegex)) {
            if (match.size() > 1) {
                std::string name = match[1].str();
                REQUIRE(name == filename);
            }
        }
    }
    SECTION("three")
    {
        std::string header = R"(attachment; filename=file.zip)";
        std::regex filenameRegex(R"(filename\s*=\s*["']?([^"']*)["']?)");
        std::smatch match;
        if (std::regex_search(header, match, filenameRegex)) {
            if (match.size() > 1) {
                std::string name = match[1].str();
                REQUIRE(name == filename);
            }
        }
    }
    SECTION("four")
    {
        std::string header = R"(attachment; filename = "file.zip")";
        std::regex filenameRegex(R"(filename\s*=\s*["']?([^"']*)["']?)");
        std::smatch match;
        if (std::regex_search(header, match, filenameRegex)) {
            if (match.size() > 1) {
                std::string name = match[1].str();
                REQUIRE(name == filename);
            }
        }
    }
}
