//
// Created by LMR on 24-8-25.
//

#include "DownloadTask.h"

DownloadTask::DownloadTask(std::string url, std::string filePath, unsigned int threadNum)
    : url_(std::move(url)), filePath_(std::move(filePath)), threadNum_(threadNum), totalSize_{}, downloadedSize_{}
{
    session_.SetUrl(cpr::Url(url_));
}

DownloadTask::~DownloadTask() {}

void DownloadTask::start()
{
    // 获取文件大小
    totalSize_ = fileSize();
    if (totalSize_ == 0) {
        std::cerr << "Failed to get file size." << std::endl;
        return;
    }

    // TODO：判断文件大小，如果小于10MB，则不进行分片下载，同时判断是否支持Range，支持才进行分片

    // 预分配文件大小
    preallocateFileSize(totalSize_);

    // 分配每个线程下载的区间
    size_t part_size = totalSize_ / threadNum_;

    // 启动线程池
    for (int i = 0; i < threadNum_; ++i) {
        size_t start = i * part_size;
        size_t end = (i == threadNum_ - 1) ? totalSize_ - 1 : start + part_size - 1;
        //TODO:线程池
        // threads_.emplace_back(&DownloadTask::downloadChunk, this, i, start, end);
    }
}

void DownloadTask::stop() {}

void DownloadTask::pause() {}

void DownloadTask::resume() {}

size_t DownloadTask::fileSize()
{
    //    cpr::Response response = session.Head();
    //    if (response.status_code != 200) {
    //        return 0;
    //    }
    //    auto length = std::stoull(response.header["Content-Length"]);
    auto length = session_.GetDownloadFileLength();
    return length;
}

void DownloadTask::preallocateFileSize(size_t fileSize)
{
    std::ofstream file(filePath_, std::ios::binary);
    file.seekp(fileSize - 1);
    file.write("", 1);
    file.close();
}

void DownloadTask::downloadChunk(size_t start, size_t end)
{
    cpr::Header headers = {{"Range", "bytes=" + std::to_string(start) + "-" + std::to_string(end)}};
    cpr::Response response = cpr::Get(cpr::Url{url_}, headers);

    if (response.status_code != 206) {
        std::cerr << "Thread " << std::this_thread::get_id()
                  << " failed to download part. Status code: " << response.status_code << std::endl;
        return;
    }

    std::lock_guard<std::mutex> lock(fileMutex_);
    std::ofstream file(filePath_, std::ios::binary | std::ios::out);
    file.seekp(start);
    file.write(response.text.c_str(), response.text.size());

    downloadedSize_ += response.text.size();
}
