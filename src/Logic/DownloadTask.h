//
// Created by LMR on 24-8-25.
//

#pragma once

#include <atomic>
#include <functional>
#include <mutex>
#include <string>
#include <thread>

#include "DownloadItem.h"
#include "Noncopyable.hpp"
#include "ThreadPool.hpp"
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
    void addHeader(const std::string& key, const std::string& value);

    DownloadItem downloadInfo();

    using ProgressCallback = std::function<void(unsigned long downloadTotal, unsigned long downloadNow)>;
    using DownloadCompleteCallback = std::function<void()>;
    void setProgressCallback(const ProgressCallback& cb);
    void setDownloadCompleteCallback(const DownloadCompleteCallback& cb);

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
    struct ChunkFile {
        uint64_t readLen;// file bytes
        uint64_t start;
        uint64_t end;
        std::string data;
        std::string chunkFilename;
    };

protected:
    HeadInfo requestFileInfoFromHead();
    HeadInfo fileSize(const cpr::Header& header);
    std::string fileName(const cpr::Response& response);
    void preallocateFileSize(uint64_t fileSize);
    void download();
    void downloadChunk(int part, uint64_t start, uint64_t end);
    bool writeCallback(const std::string_view& data, intptr_t userdata);
    bool progressCallback(long downloadTotal, long downloadNow, long uploadTotal, long uploadNow, intptr_t userdata);
    bool isDownloadComplete();
    void mergeChunkFiles();

private:
    std::string url_;
    std::string filePath_;       // save the file path
    std::string filename_;       // file name, excluding the path.
    std::string tmpFilenamePath_;// tmp file path
    unsigned int threadNum_;
    cpr::Session session_;
    cpr::Header header_;
    std::atomic<uint64_t> totalSize_;
    std::atomic<uint64_t> downloadedSize_;
    Status status_;
    std::mutex statsMutex_;
    ProgressCallback progressCallback_;
    DownloadCompleteCallback downloadCompleteCallback;
    ThreadPool pool;
};
