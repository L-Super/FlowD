//
// Created by LMR on 24-8-25.
//

#include "DownloadManager.h"
#include "DownloadTask.h"

DownloadManager::DownloadManager() {}

DownloadManager::~DownloadManager() {}

void DownloadManager::addTask(const std::string& url, const std::string& filePath, unsigned int threadNum)
{
    tasks_[url] = std::make_unique<DownloadTask>(url, filePath, threadNum);
    tasks_[url]->start();
}

void DownloadManager::pauseTask(const std::string& taskID)
{

    if (tasks_.find(taskID) != tasks_.end()) {
        tasks_[taskID]->pause();
    }
}

void DownloadManager::resumeTask(const std::string& taskID)
{

    if (tasks_.find(taskID) != tasks_.end()) {
        tasks_[taskID]->resume();
    }
}

void DownloadManager::removeTask(const std::string& taskID)
{

    if (tasks_.find(taskID) != tasks_.end()) {
        tasks_[taskID]->stop();
        tasks_.erase(taskID);
    }
}
