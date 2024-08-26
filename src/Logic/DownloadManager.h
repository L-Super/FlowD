//
// Created by LMR on 24-8-25.
//

#pragma once
#include <map>
#include <memory>
#include <string>
#include <thread>

#include "Noncopyable.hpp"

class DownloadTask;
class DownloadManager : private Noncopyable {
public:
    static DownloadManager& GetInstance()
    {
        static DownloadManager instance;
        return instance;
    }

    void addTask(const std::string& url, const std::string& filePath,
                 unsigned int threadNum = std::thread::hardware_concurrency());
    void pauseTask(const std::string& taskID);
    void resumeTask(const std::string& taskID);
    void removeTask(const std::string& taskID);

private:
    DownloadManager();
    ~DownloadManager();

private:
    std::map<std::string, std::unique_ptr<DownloadTask>> tasks_;
};
