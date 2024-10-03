//
// Created by LMR on 24-8-25.
//

#include "DownloadManager.h"
#include "DownloadTask.h"

DownloadManager::DownloadManager() {}

DownloadManager::~DownloadManager() {
    for (const auto& task:tasks_) {
        task.second->stop();
    }
    tasks_.clear();
}

size_t DownloadManager::addTask(const std::string& url, const std::string& filePath, unsigned int threadNum)
{
    auto urlHash = std::hash<std::string>{}(url);
    tasks_[urlHash] = std::make_unique<DownloadTask>(url, filePath, threadNum);
    return urlHash;
}

void DownloadManager::setProgressCallback(size_t taskID, const ProgressCallback& cb)
{
    tasks_[taskID]->setProgressCallback(cb);
}

void DownloadManager::setDownloadCompleteCallback(size_t taskID, const DownloadCompleteCallback& cb)
{
    tasks_[taskID]->setDownloadCompleteCallback(cb);
}

void DownloadManager::startTask(size_t taskID)
{
    tasks_[taskID]->start();
}

void DownloadManager::pauseTask(size_t taskID)
{
    if (tasks_.find(taskID) != tasks_.end()) {
        tasks_[taskID]->pause();
    }
}

void DownloadManager::resumeTask(size_t taskID)
{
    if (tasks_.find(taskID) != tasks_.end()) {
        tasks_[taskID]->resume();
    }
}

void DownloadManager::removeTask(size_t taskID)
{
    if (tasks_.find(taskID) != tasks_.end()) {
        tasks_[taskID]->stop();
        tasks_.erase(taskID);
    }
}

std::optional<DownloadItem> DownloadManager::downloadTaskInfo(size_t taskID)
{
    if (tasks_.find(taskID) != tasks_.end()) {
        return tasks_[taskID]->downloadInfo();
    }
    return std::nullopt;
}
