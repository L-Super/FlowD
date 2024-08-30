//
// Created by LMR on 24-8-25.
//

#include "DownloadTask.h"
#include "spdlog/spdlog.h"

#include <filesystem>

namespace {
    constexpr unsigned int KB = 1 * 1024;
    constexpr unsigned int MB = 1 * 1024 * 1024;
}// namespace

namespace fs = std::filesystem;

DownloadTask::DownloadTask(std::string url, std::string filePath, unsigned int threadNum)
    : url_(std::move(url)), filePath_(std::move(filePath)), threadNum_(threadNum), totalSize_{}, downloadedSize_{},
      status_(Status::STOP)
{
#if defined(WINDOWS_OS)
    header_ = {{"user-agent", "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like "
                              "Gecko) Chrome/128.0.0.0 Safari/537.36 Edg/128.0.0.0"}};
#elif defined(MAC_OS)
    header_ = {{"user-agent", "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/537.36 (KHTML, like Gecko) "
                      "Chrome/127.0.0.0 Safari/537.36 Edg/127.0.0.0"}};
#elif defined(LINUX_OS)
#endif

    session_.SetUrl(cpr::Url(url_));
}

DownloadTask::~DownloadTask() {}

void DownloadTask::start()
{
    // get file size
    auto [totalSize, supportRange, filename] = requestFileInfoFromHead();
    totalSize_ = totalSize;
    if (totalSize_ == 0) {
        spdlog::error("Failed to get file size.");
        return;
    }
    filename_ = filename;
    tmpFilenamePath_ = filePath_ + "/" + filename + ".tmp";
    status_ = Status::RUNNING;

    fs::path dirPath = fs::path(tmpFilenamePath_).parent_path();
    // If the directory does not exist, create it
    if (!dirPath.empty() && !fs::exists(dirPath)) {
        fs::create_directories(dirPath);
    }

    // Preallocate file size.
    preallocateFileSize(totalSize_);

    // If the file is smaller than 10MB or does not support Range, download it directly.
    if (totalSize_ < 10 * MB || !supportRange) {
        spdlog::info("Start single thread download");
        auto future = std::async(std::launch::async, [&] {
            download();
        });
        return;
    }

    spdlog::info("Start mutil thread download");

    // Allocate the compartment for each thread's download
    size_t part_size = totalSize_ / threadNum_;

    // launch the thread pool
    threadPool.SetMinThreadNum(threadNum_);
    //    threadPool.SetMaxThreadNum(threadNum_);
    threadPool.Start();
    for (int i = 0; i < threadNum_; ++i) {
        size_t start = i * part_size;
        size_t end = (i == threadNum_ - 1) ? totalSize_ - 1 : start + part_size - 1;

        //TODO:
        // How to check it download finish and resume
        threadPool.Submit([&] {
            downloadChunk(start, end);
        });
    }
    threadPool.Wait();
}

void DownloadTask::stop()
{
    spdlog::info("Download stop");
    std::lock_guard<std::mutex> lg(statsMutex_);
    status_ = Status::STOP;
    threadPool.Stop();
}

void DownloadTask::pause()
{
    spdlog::info("Download pause");
    std::lock_guard<std::mutex> lg(statsMutex_);
    status_ = Status::PAUSE;
    threadPool.Pause();
}

void DownloadTask::resume()
{
    spdlog::info("Download resume");
    std::lock_guard<std::mutex> lg(statsMutex_);
    status_ = Status::RUNNING;
    threadPool.Resume();
}

std::map<std::string, std::string> DownloadTask::header()
{
    std::map<std::string, std::string> m;
    for (const auto& pair: header_) {
        auto [k, v] = pair;
        m[k] = v;
    }
    return m;
}

void DownloadTask::setHeader(const std::map<std::string, std::string>& header)
{
    for (const auto& pair: header) {
        auto [k, v] = pair;
        header_[k] = v;
    }
}

void DownloadTask::addHeader(const std::string& key, const std::string& value)
{
    header_[key] = value;
}

DownloadTask::HeadInfo DownloadTask::requestFileInfoFromHead()
{
    session_.SetHeader(header_);
    //TODO: Maybe use GetDownloadFileLength first
    //see:https://github.com/libcpr/cpr/pull/599
    cpr::Response response = session_.Head();
    if (response.status_code != 200) {
        spdlog::error("Request head failed.");
        return {};
    }
    HeadInfo fileInfo = fileSize(response.header);
    fileInfo.filename = fileName(response);

    spdlog::info("Request head. Accept-Ranges support:{} file name:{} file length:{}", fileInfo.supportRange,
                 fileInfo.filename, fileInfo.length);
    return fileInfo;
}

DownloadTask::HeadInfo DownloadTask::fileSize(const cpr::Header& header)
{
    unsigned long length{};
    if (auto search = header.find("Content-Length"); search != header.end()) {
        length = std::stoul(search->second);
    }

    if (auto search = header.find("Accept-Ranges"); search != header.end()) {
        auto acceptRange = search->second;
        // not support Accept-Ranges:
        // haven't Accept-Ranges header or value is none
        if (acceptRange == "bytes") {
            return {length, true};
        }
        else {
            return {length, false};
        }
    }
    // auto length = session_.GetDownloadFileLength();
    return {};
}

std::string DownloadTask::fileName(const cpr::Response& response)
{
    auto headers = response.header;
    auto url = response.url.str();
    // First, try to get it from the Content-Disposition header.
    auto getFilenameFromHeader = [&headers]() -> std::string {
        if (auto it = headers.find("Content-Disposition"); it != headers.end()) {
            std::string header = it->second;
            size_t filenamePos = header.find("filename=");
            if (filenamePos != std::string::npos) {
                size_t start = filenamePos + 9;// length of "filename="
                size_t end = header.find('"', start + 1);
                return header.substr(start + 1, end - start - 1);
            }
        }
        return {};
    };
    auto getFilenameFromUrl = [&url]() -> std::string {
        size_t pos = url.find_last_of('/');
        if (pos != std::string::npos) {
            return url.substr(pos + 1);
        }
        return {};
    };

    auto filename = getFilenameFromHeader();
    if (filename.empty()) {
        spdlog::warn("Get filename from header failed");
        filename = getFilenameFromUrl();
    }
    if (filename.empty()) {
        spdlog::warn("Get filename from url failed");
        return "downloading";
    }
    spdlog::info("Get filename success. File name:{}", filename);
    return filename;
}

void DownloadTask::preallocateFileSize(size_t fileSize)
{
    // TODO: Check it whether the disk space is enough
    spdlog::info("Preallocate file size {}KB", fileSize / KB);

    std::ofstream file(tmpFilenamePath_, std::ios::binary);
    file.seekp(fileSize - 1);
    file.write("", 1);
    file.close();
}

void DownloadTask::download()
{
    if (status_ != Status::RUNNING)
        return;

    std::ofstream file(tmpFilenamePath_, std::ios::binary | std::ios::out);

    session_.SetProgressCallback(cpr::ProgressCallback(
            [this](long downloadTotal, long downloadNow, long uploadTotal, long uploadNow, auto userdata) {
                return progressCallback(downloadTotal, downloadNow, uploadTotal, uploadNow, userdata);
            }));
    auto response = session_.Download(file);
    if (response.downloaded_bytes == totalSize_) {
        fs::path tempPath(tmpFilenamePath_);
        // remove .tmp suffix
        fs::path finalPath = tempPath.parent_path() / tempPath.stem();
        std::filesystem::rename(tempPath, finalPath);
        spdlog::info("Download small file finish. file path:{}", finalPath.string());
        status_ = Status::STOP;
        if (downloadCompleteCallback) {
            downloadCompleteCallback();
        }
    }
    else {
        spdlog::error("Download small file failed. code:{} error reason: {}", response.status_code, response.reason);
    }
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

    File f;
    f.read_len = start;
    f.start = start;
    cpr::WriteCallback writeCallbackFunc(
            [this, start](const std::string_view& data, intptr_t userdata) {
                // Return true on success, or false to cancel the transfer.
                if (status_ != Status::RUNNING) {
                    // TODO:return false will cancel download. try to pause
                    return false;
                }
                return writeCallback(data, userdata, start);
            },
            reinterpret_cast<intptr_t>(&f));

    cpr::Response response =
            cpr::Get(cpr::Url{url_}, header_, cpr::Range(start, end), progressCallbackFunc, writeCallbackFunc);

    std::ostringstream oss;
    oss << std::this_thread::get_id();
    if (response.status_code != 206) {
        spdlog::error("Thread {} failed to download part. Status code: {}", oss.str(), response.status_code);
        return;
    }
    else {
        spdlog::info("Thread {} download range[{}:{}] file finished.", oss.str(), start, end);

        if (isDownloadComplete()) {
            fs::path tempPath(tmpFilenamePath_);
            // remove .tmp suffix
            fs::path finalPath = tempPath.parent_path() / tempPath.stem();
            std::lock_guard<std::mutex> lg(statsMutex_);
            std::filesystem::rename(tempPath, finalPath);
            status_ = Status::STOP;
        }
        if (downloadCompleteCallback) {
            downloadCompleteCallback();
        }
    }
}

bool DownloadTask::writeCallback(const std::string_view& data, intptr_t userdata, size_t start)
{
    // FIXME: write file wrong
    File* pf = reinterpret_cast<File*>(userdata);

    try {
        std::ofstream file(tmpFilenamePath_, std::ios::binary | std::ios::out);

        file.seekp(static_cast<long long>(pf->read_len));
        file.write(data.data(), data.size());

        downloadedSize_ += data.size();
        pf->read_len += data.size();
    }
    catch (const std::exception& e) {
        spdlog::error("Write file failed. Exception:{}", e.what());
    }

    spdlog::info("Write callback. start:{} read len:{} downloaded size:{}", start, pf->read_len,
                 downloadedSize_.load());

    return true;
}

bool DownloadTask::progressCallback(long downloadTotal, long downloadNow, long uploadTotal, long uploadNow,
                                    intptr_t userdata)
{
    (void) uploadTotal;
    (void) uploadNow;
    (void) userdata;

    if (progressCallback_) {
        spdlog::info("Execute progress callback");
        progressCallback_(totalSize_.load(), downloadedSize_.load());
    }

    spdlog::info("Download {}/{} bytes.", downloadNow, downloadTotal);

    return true;
}

void DownloadTask::setProgressCallback(ProgressCallback& cb)
{
    progressCallback_ = cb;
}

void DownloadTask::setDownloadCompleteCallback(DownloadTask::DownloadCompleteCallback& cb)
{
    downloadCompleteCallback = cb;
}

bool DownloadTask::isDownloadComplete()
{
    if (totalSize_ == downloadedSize_)
        return true;
    return false;
}
