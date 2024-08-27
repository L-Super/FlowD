//
// Created by LMR on 24-8-25.
//

#include "DownloadManager.h"
#include "DownloadTask.h"

DownloadManager::DownloadManager() {}

DownloadManager::~DownloadManager() {}

size_t DownloadManager::addTask(const std::string& url, const std::string& filePath, unsigned int threadNum)
{
    auto urlHash = std::hash<std::string>{}(url);
    tasks_[urlHash] = std::make_unique<DownloadTask>(url, filePath, threadNum);
    tasks_[urlHash]->start();
    return urlHash;
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
