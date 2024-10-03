#pragma once
#include <string>

struct DownloadItem {
    enum class DownloadStatus {
        Downloading,// 下载中
        Pause,      // 暂停，未下载完成
        Stop        //停止，下载完成
    };

    std::string filename;
    std::string filenamePath;
    std::string url;
    DownloadStatus status;

    int64_t downloadedBytes{};
    int64_t totalBytes{};
    int64_t remainTime{};

    double downloadSpeed{};
};
