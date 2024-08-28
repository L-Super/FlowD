//
// Created by LMR on 24-8-25.
//

#pragma once

#include <atomic>
#include <iostream>
#include <string>
#include <thread>
#include <functional>

#include "Noncopyable.hpp"
#include "cpr/cpr.h"

class DownloadTask : private Noncopyable {
public:
    DownloadTask(std::string url, std::string filePath, unsigned int threadNum);
    ~DownloadTask();
    void start();
    void stop();
    void pause();
    void resume();

    using ProgressCallback = std::function<void(long downloadTotal, long downloadNow)>;
    void setProgressCallback(ProgressCallback& cb);

protected:
    struct HeadInfo {
        size_t length{};
        bool supportRange{};
    };
    enum class Status {
        STOP,
        RUNNING,
        PAUSE,
    };

protected:
    HeadInfo fileSize();
    void preallocateFileSize(size_t fileSize);
    void download();
    void downloadChunk(size_t start, size_t end);
    bool writeCallback(const std::string_view& data, intptr_t userdata, size_t start);
    bool progressCallback(long downloadTotal, long downloadNow, long uploadTotal, long uploadNow, intptr_t userdata);

private:
    std::string url_;
    std::string filePath_;
    std::string tmpFilePath_;
    unsigned int threadNum_;
    cpr::Session session_;
    std::atomic<unsigned long> totalSize_;
    std::atomic<unsigned long> downloadedSize_;
    Status status_;
    ProgressCallback progressCallback_;
};