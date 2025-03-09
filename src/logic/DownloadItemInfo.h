#pragma once
#include <atomic>
#include <string>
#include <thread>

struct DownloadItemInfo {
    enum class DownloadStatus {
        Downloading, // 下载中
        Pause,       // 暂停，未下载完成
        Stop         // 停止，下载完成
    };

    std::string filename;     // 文件名
    std::string saveDir;      // 保存文件目录路径
    std::string filenamePath; // 保存文件路径
    std::string url;
    std::string userAgent; // 用户代理
    DownloadStatus status{DownloadStatus::Stop};

    std::atomic<uint64_t> downloadedBytes{}; // 已下载 byte
    std::atomic<uint64_t> totalBytes{};      // 总大小
    std::atomic<double> remainTime{};        // 剩余时间
    std::atomic<uint64_t> downloadSpeed{};   // 下载速度

    unsigned int threadNum{std::thread::hardware_concurrency()}; // 指定下载线程数

    DownloadItemInfo() = default;
    DownloadItemInfo(const DownloadItemInfo& other);
    DownloadItemInfo(DownloadItemInfo&& other) noexcept;
    DownloadItemInfo& operator=(const DownloadItemInfo& other);
    DownloadItemInfo& operator=(DownloadItemInfo&& other) noexcept;
};
