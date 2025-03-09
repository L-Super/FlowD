
#include "DownloadItemInfo.h"

DownloadItemInfo::DownloadItemInfo(const DownloadItemInfo& other)
    : filename(other.filename), saveDir(other.saveDir), filenamePath(other.filenamePath), url(other.url),
      status(other.status), downloadedBytes(other.downloadedBytes.load()), totalBytes(other.totalBytes.load()),
      remainTime(other.remainTime.load()), downloadSpeed(other.downloadSpeed.load()), threadNum(other.threadNum)
{}

DownloadItemInfo::DownloadItemInfo(DownloadItemInfo&& other) noexcept
    : filename(std::move(other.filename)), saveDir(std::move(other.saveDir)),
      filenamePath(std::move(other.filenamePath)), url(std::move(other.url)), status(other.status),
      downloadedBytes(other.downloadedBytes.load()), totalBytes(other.totalBytes.load()),
      remainTime(other.remainTime.load()), downloadSpeed(other.downloadSpeed.load()), threadNum(other.threadNum)
{
    // 将原对象的原子变量设置为默认值
    other.downloadedBytes.store(0);
    other.totalBytes.store(0);
    other.remainTime.store(0);
    other.downloadSpeed.store(0);
    other.status = DownloadStatus::Stop; // 可选：将状态设置为初始状态
}

DownloadItemInfo& DownloadItemInfo::operator=(const DownloadItemInfo& other)
{
    if (this != &other) { // 防止自赋值
        filename = other.filename;
        saveDir = other.saveDir;
        filenamePath = other.filenamePath;
        url = other.url;
        status = other.status;
        threadNum = other.threadNum;

        downloadedBytes.store(other.downloadedBytes.load());
        totalBytes.store(other.totalBytes.load());
        remainTime.store(other.remainTime.load());
        downloadSpeed.store(other.downloadSpeed.load());
    }
    return *this;
}

DownloadItemInfo& DownloadItemInfo::operator=(DownloadItemInfo&& other) noexcept
{
    if (this != &other) {
        filename = std::move(other.filename);
        saveDir = std::move(other.saveDir);
        filenamePath = std::move(other.filenamePath);
        url = std::move(other.url);
        status = other.status;
        downloadedBytes.store(other.downloadedBytes.load());
        totalBytes.store(other.totalBytes.load());
        remainTime.store(other.remainTime.load());
        downloadSpeed.store(other.downloadSpeed.load());
        threadNum = other.threadNum;

        // 将原对象的原子变量设置为默认值
        other.downloadedBytes.store(0);
        other.totalBytes.store(0);
        other.remainTime.store(0);
        other.downloadSpeed.store(0);
        other.status = DownloadStatus::Stop; // 可选：将状态设置为初始状态
    }
    return *this;
}
