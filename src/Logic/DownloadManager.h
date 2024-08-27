//
// Created by LMR on 24-8-25.
//

#pragma once
#include <memory>
#include <string>
#include <thread>
#include <unordered_map>

#include "Noncopyable.hpp"

class DownloadTask;
class DownloadManager : private Noncopyable {
public:
    static DownloadManager& GetInstance()
    {
        static DownloadManager instance;
        return instance;
    }

    [[nodiscard]] std::size_t addTask(const std::string& url, const std::string& filePath,
                                      unsigned int threadNum = std::thread::hardware_concurrency());
    void pauseTask(size_t taskID);
    void resumeTask(size_t taskID);
    void removeTask(size_t taskID);

private:
    DownloadManager();
    ~DownloadManager();

private:
    std::unordered_map<size_t, std::unique_ptr<DownloadTask>> tasks_;
};
