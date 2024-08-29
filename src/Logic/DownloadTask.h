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

    std::map<std::string, std::string> header();
    void setHeader(const std::map<std::string, std::string>& header);
    void addHeader(const std::string &key,const std::string &value);

    using ProgressCallback = std::function<void(unsigned long downloadTotal, unsigned long downloadNow)>;
    using DownloadCompleteCallback = std::function<void()>;
    void setProgressCallback(ProgressCallback& cb);
    void setDownloadCompleteCallback(DownloadCompleteCallback& cb);

protected:
    struct HeadInfo {
        unsigned long length{};
        bool supportRange{};
        std::string filename;
    };
    enum class Status {
        STOP,
        RUNNING,
        PAUSE,
    };
    struct File
    {
        int64_t read_len;  // file bytes
        int64_t start;
    };

protected:
    HeadInfo requestFileInfoFromHead();
    HeadInfo fileSize(const cpr::Header& header);
    std::string fileName(const cpr::Response& response);
    void preallocateFileSize(size_t fileSize);
    void download();
    void downloadChunk(size_t start, size_t end);
    bool writeCallback(const std::string_view& data, intptr_t userdata, size_t start);
    bool progressCallback(long downloadTotal, long downloadNow, long uploadTotal, long uploadNow, intptr_t userdata);
    bool isDownloadComplete();

private:
    std::string url_;
    std::string filePath_; // save the file path
    std::string filename_; // file name, excluding the path.
    std::string tmpFilenamePath_; // tmp file path
    unsigned int threadNum_;
    cpr::Session session_;
    cpr::Header header_;
    std::atomic<unsigned long> totalSize_;
    std::atomic<unsigned long> downloadedSize_;
    Status status_;
    ProgressCallback progressCallback_;
    DownloadCompleteCallback downloadCompleteCallback;
};