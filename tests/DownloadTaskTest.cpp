#include <catch2/catch_test_macros.hpp>
#include <regex>
#include <string>

TEST_CASE("filename", "[test1]")
{
    std::string filename = "file.zip";
    auto getFilename = [](const std::string& header) -> std::string {
        std::regex filenameRegex(R"(filename\s*=\s*["']?([^"']*)["']?)");
        std::smatch match;
        if (std::regex_search(header, match, filenameRegex)) {
            if (match.size() > 1) {
                std::string name = match[1].str();
                return name;
            }
        }
        return {};
    };
    SECTION("regex-one")
    {
        std::string header = R"(attachment; filename="file.zip")";
        REQUIRE(getFilename(header) == filename);
    }
    SECTION("regex-two")
    {
        std::string header = R"(attachment; filename='file.zip')";
        REQUIRE(getFilename(header) == filename);
    }
    SECTION("regex-three")
    {
        std::string header = R"(attachment; filename=file.zip)";
        REQUIRE(getFilename(header) == filename);
    }
    SECTION("regex-four")
    {
        std::string header = R"(attachment; filename = "file.zip")";
        REQUIRE(getFilename(header) == filename);
    }

    auto getFilenameFormUrl = [](const std::string& url) -> std::string {
        size_t pos = url.find_last_of('/');
        if (pos != std::string::npos) {
            return url.substr(pos + 1);
        }
        return {};
    };
    SECTION("url-one")
    {
        std::string url = R"(https://www.xxx.com/2024/file.zip)";
        REQUIRE(getFilenameFormUrl(url) == filename);
    }
}
