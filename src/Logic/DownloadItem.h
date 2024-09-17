#pragma once
#include <string>

struct DownloadItem {
    enum class DownloadStatus { Downloading, Pause, Stop };

    std::string filename;
    std::string filenamePath;
    std::string url;
    DownloadStatus status;

    int64_t downloadedBytes{};
    int64_t totalBytes{};
    int64_t remainTime{};

    double downloadSpeed{};
};
