//
// Created by LMR on 24-8-25.
//

#pragma once

#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>

#include "DownloadItemInfo.h"
#include "Noncopyable.hpp"

class DownloadTask;
class DownloadManager : private Noncopyable {
public:
    static DownloadManager& instance()
    {
        static DownloadManager instance;
        return instance;
    }

    [[nodiscard]] std::size_t addTask(const DownloadItemInfo& item);

    using ProgressCallback =
        std::function<void(unsigned long total, unsigned long downloaded, unsigned long speed, double remainTime)>;
    using DownloadCompleteCallback = std::function<void()>;
    void setProgressCallback(size_t taskID, const ProgressCallback& cb);
    void setDownloadCompleteCallback(size_t taskID, const DownloadCompleteCallback& cb);

    void startTask(size_t taskID);
    void pauseTask(size_t taskID);
    void resumeTask(size_t taskID);
    void removeTask(size_t taskID);
    std::optional<DownloadItemInfo> downloadTaskInfo(size_t taskID);

private:
    DownloadManager();
    ~DownloadManager();

private:
    std::unordered_map<size_t, std::unique_ptr<DownloadTask>> tasks_;
};
