//
// Created by LMR on 24-8-25.
//

#pragma once

#include <atomic>
#include <functional>
#include <mutex>
#include <string>
#include <thread>

#include "DownloadItemInfo.h"
#include "Noncopyable.hpp"
#include "ThreadPool.hpp"
#include "cpr/cpr.h"

class DownloadTask : private Noncopyable {
public:
    DownloadTask(const DownloadItemInfo& item);
    ~DownloadTask();
    void start();
    void stop();
    void pause();
    void resume();

    std::map<std::string, std::string> header();
    void setHeader(const std::map<std::string, std::string>& header);
    void addHeader(const std::string& key, const std::string& value);

    DownloadItemInfo downloadInfo();

    using ProgressCallback =
            std::function<void(unsigned long total, unsigned long downloaded, unsigned long speed, double remainTime)>;
    using DownloadCompleteCallback = std::function<void()>;
    void setProgressCallback(const ProgressCallback& cb);
    void setDownloadCompleteCallback(const DownloadCompleteCallback& cb);

protected:
    struct HeadInfo {
        unsigned long length{};
        bool supportRange{};
        std::string filename;
    };

    struct ChunkFile {
        uint64_t readLen{};// file bytes
        uint64_t start{};
        uint64_t end{};
        std::string data;
    };

protected:
    HeadInfo requestFileInfoFromHead();
    unsigned long fileSize(const cpr::Header& header);
    bool isAcceptRange(const cpr::Header& header);
    std::string fileName(const cpr::Response& response);
    void preallocateFileSize(uint64_t fileSize);
    void download();
    void downloadChunk(int part, uint64_t start, uint64_t end);
    bool writeCallback(const std::string_view& data, intptr_t userdata);
    bool progressCallback(long downloadTotal, long downloadNow, long uploadTotal, long uploadNow, intptr_t userdata);
    bool isDownloadComplete();
    void mergeChunkFiles();
    void speedAndRemainingTimeCalculate();

private:
    DownloadItemInfo itemInfo_;
    std::string tmpFilenamePath_;// tmp file path
    cpr::Session session_;
    cpr::Header header_;

    std::mutex statsMutex_;
    ProgressCallback progressCallback_;
    DownloadCompleteCallback downloadCompleteCallback_;
    ThreadPool pool_;
};
