//
// Created by LMR on 24-8-25.
//

#pragma once

#include <iostream>
#include <string>
#include <thread>

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

protected:
    size_t fileSize();
    void preallocateFileSize(size_t fileSize);
    void downloadChunk(size_t start, size_t end);

private:
    std::string url_;
    std::string filePath_;
    unsigned int threadNum_;
    cpr::Session session_;
    size_t totalSize_;
    std::atomic<size_t> downloadedSize_;
    std::mutex fileMutex_;
};