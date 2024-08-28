//
// Created by LMR on 24-8-25.
//

#include "DownloadTask.h"
#include "spdlog/spdlog.h"

#include <filesystem>

namespace {
    constexpr unsigned int KB = 1 * 1024 * 1024;
    constexpr unsigned int MB = 1 * 1024 * 1024 * 1024;
}// namespace

DownloadTask::DownloadTask(std::string url, std::string filePath, unsigned int threadNum)
    : url_(std::move(url)), filePath_(std::move(filePath)), threadNum_(threadNum), totalSize_{}, downloadedSize_{},
      status_(Status::STOP)
{
    tmpFilePath_ = filePath_ + ".tmp";
    session_.SetUrl(cpr::Url(url_));
    session_.SetHeader({{"user-agent", "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like "
                                       "Gecko) Chrome/128.0.0.0 Safari/537.36 Edg/128.0.0.0"}});
}

DownloadTask::~DownloadTask() {}

void DownloadTask::start()
{
    // get file size
    auto [totalSize, supportRange] = fileSize();
    totalSize_ = totalSize;
    if (totalSize_ == 0) {
        spdlog::error("Failed to get file size.");
        return;
    }
    status_ = Status::RUNNING;

    // Preallocate file size.
    preallocateFileSize(totalSize_);

    // If the file is smaller than 10MB or does not support Range, download it directly.
    if (totalSize_ < 10 * MB || !supportRange) {
        // TODO：
        auto future = std::async(std::launch::async, [&] {
            download();
        });
        return;
    }

    // Allocate the compartment for each thread's download
    size_t part_size = totalSize_ / threadNum_;

    // launch the thread pool
    for (int i = 0; i < threadNum_; ++i) {
        size_t start = i * part_size;
        size_t end = (i == threadNum_ - 1) ? totalSize_ - 1 : start + part_size - 1;
        cpr::ThreadPool threadPool;
        //TODO: thread pool
        // How to check it download finish
        // threads_.emplace_back(&DownloadTask::downloadChunk, this, i, start, end);
    }
}

void DownloadTask::stop()
{
    spdlog::info("Download stop");
    status_ = Status::STOP;
}

void DownloadTask::pause()
{
    spdlog::info("Download pause");
    status_ = Status::PAUSE;
}

void DownloadTask::resume()
{
    spdlog::info("Download resume");
    status_ = Status::RUNNING;
}

DownloadTask::HeadInfo DownloadTask::fileSize()
{
    //TODO: Maybe use GetDownloadFileLength first
    //see:https://github.com/libcpr/cpr/pull/599
    cpr::Response response = session_.Head();
    if (response.status_code != 200) {
        spdlog::error("Request head failed.");
        return {};
    }
    HeadInfo info;
    auto length = std::stoull(response.header["Content-Length"]);
    info.length = length;
    auto acceptRange = response.header["Accept-Ranges"];
    // not support Accept-Ranges:
    // haven't Accept-Ranges header or value is none
    if (acceptRange == "bytes") {
        info.supportRange = true;
    }
    else {
        info.supportRange = false;
    }
    spdlog::info("Request head. Accept-Ranges support is {} file length:{}", info.supportRange, info.length);
    // auto length = session_.GetDownloadFileLength();
    return info;
}

void DownloadTask::preallocateFileSize(size_t fileSize)
{
    std::ofstream file(tmpFilePath_, std::ios::binary);
    file.seekp(fileSize - 1);
    file.write("", 1);
    file.close();
}

void DownloadTask::download()
{
    if (status_ != Status::RUNNING)
        return;
    //TODO: maybe add write callback
    std::ofstream file(tmpFilePath_, std::ios::binary | std::ios::out);

    session_.SetProgressCallback(cpr::ProgressCallback(
            [this](long downloadTotal, long downloadNow, long uploadTotal, long uploadNow, auto userdata) {
                return progressCallback(downloadTotal, downloadNow, uploadTotal, uploadNow, userdata);
            }));
    auto response = session_.Download(file);
    if (response.downloaded_bytes == totalSize_) {
        spdlog::info("{} download finish. file path:{}", __func__, filePath_);
        std::filesystem::rename(tmpFilePath_, filePath_);
    }

    status_ = Status::STOP;
}

void DownloadTask::downloadChunk(size_t start, size_t end)
{
    if (status_ != Status::RUNNING)
        return;
    // cpr::Header headers = {{"Range", "bytes=" + std::to_string(start) + "-" + std::to_string(end)}};
    cpr::ProgressCallback progressCallbackFunc(
            [this](long downloadTotal, long downloadNow, long uploadTotal, long uploadNow, auto userdata) {
                return progressCallback(downloadTotal, downloadNow, uploadTotal, uploadNow, userdata);
            });
    cpr::WriteCallback writeCallbackFunc([this, start](const std::string_view& data, auto userdata) {
        // Return true on success, or false to cancel the transfer.
        //TODO: if return false, cancel download or pause?
        if (status_ != Status::RUNNING)
            return false;
        return writeCallback(data, userdata, start);
    });
    cpr::Response response = cpr::Get(cpr::Url{url_}, cpr::Range(start, end), progressCallbackFunc, writeCallbackFunc);

    if (response.status_code != 206) {
        std::ostringstream oss;
        oss << std::this_thread::get_id();
        spdlog::error("Thread {} failed to download part. Status code: {}", oss.str(), response.status_code);
        return;
    }
    status_ = Status::STOP;
}

bool DownloadTask::writeCallback(const std::string_view& data, intptr_t userdata, size_t start)
{
    std::ofstream file(tmpFilePath_, std::ios::binary | std::ios::out);
    file.seekp(static_cast<long long>(start));
    file.write(data.data(), data.size());

    downloadedSize_ += data.size();
    spdlog::info("Write callback. start:{} downloaded size:{}", start, downloadedSize_.load());

    // TODO:
    if (progressCallback_) {
        spdlog::info("execute progress callback");
        progressCallback_(totalSize_.load(), downloadedSize_.load());
    }

    return false;
}

bool DownloadTask::progressCallback(long downloadTotal, long downloadNow, long uploadTotal, long uploadNow,
                                    intptr_t userdata)
{
    (void) uploadTotal;
    (void) uploadNow;
    (void) userdata;

    spdlog::info("Download {}/{} bytes.", downloadNow, downloadTotal);

    return true;
}

void DownloadTask::setProgressCallback(ProgressCallback& cb)
{
    progressCallback_ = cb;
}
