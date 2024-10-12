//
// Created by LMR on 24-8-25.
//

#include "DownloadTask.h"
#include "Logger.hpp"

#include "mio/mio.hpp"

#include <filesystem>
#include <regex>

namespace fs = std::filesystem;

namespace {
    constexpr unsigned int KB = 1 * 1024;
    constexpr unsigned int MB = 1 * 1024 * 1024;

    std::string generateUniqueFilename(const std::string& filePath)
    {
        std::string newFilePath = filePath;
        std::string directory = fs::path(filePath).parent_path().string();
        std::string base_filename = fs::path(filePath).stem().string();
        std::string extension = fs::path(filePath).extension().string();
        int i = 1;


        // 循环直到找到一个不存在的文件名
        while (fs::exists(newFilePath)) {
            // 构造新的文件名，格式为：xxx(i).ext
            newFilePath = directory + "/" + base_filename + "(" + std::to_string(i++) + ")" + extension;
        }
        return newFilePath;
    }
}// namespace

DownloadTask::DownloadTask(std::string url, std::string filePath, unsigned int threadNum)
    : url_(std::move(url)), saveFilePath_(std::move(filePath)), threadNum_(threadNum), totalSize_{}, downloadedSize_{},
      speed_{}, remainTime_{}, status_(Status::STOP), pool_(threadNum + 1)
{
#if defined(WINDOWS_OS)
    header_ = {{"user-agent", "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like "
                              "Gecko) Chrome/128.0.0.0 Safari/537.36 Edg/128.0.0.0"}};
#elif defined(MAC_OS)
    header_ = {{"user-agent", "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/537.36 (KHTML, like Gecko) "
                              "Chrome/127.0.0.0 Safari/537.36 Edg/127.0.0.0"}};
#elif defined(LINUX_OS)
    header_ = {{"user-agent", "Mozilla/5.0 (X11; Linux x86_64; rv:125.0) Gecko/20100101 Firefox/125.0"}};
#endif
    header_["Connection"] = "Keep-Alive";

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
        // Unable to get file size, can try downloading
    }
    filename_ = filename;
    status_ = Status::RUNNING;
    tmpFilenamePath_ = generateUniqueFilename(saveFilePath_ + "/" + filename_) + ".part";

    fs::path dirPath = fs::path(saveFilePath_);
    // If the directory does not exist, create it
    if (!dirPath.empty() && !fs::exists(dirPath)) {
        fs::create_directories(dirPath);
    }

    // If the file is smaller than 10MB or does not support Range, download it directly.
    if (totalSize_ < 10 * MB || !supportRange) {
        spdlog::info("Start single thread download");
        pool_.enqueue(&DownloadTask::download, this);
        return;
    }

    spdlog::info("Start mutil thread download.");
    // Preallocate file size.
    preallocateFileSize(totalSize_);

    // Allocate the compartment for each thread's download
    uint64_t part_size = totalSize_ / threadNum_;
    spdlog::info("Thread count:{} part size:{}", threadNum_, part_size);

    // launch the thread pool
    std::vector<std::future<void>> results;
    for (int i = 0; i < threadNum_; ++i) {
        uint64_t start = i * part_size;
        uint64_t end = (i == threadNum_ - 1) ? totalSize_ - 1 : start + part_size - 1;

        results.emplace_back(pool_.enqueue([this, i, start, end] {
            downloadChunk(i, start, end);
        }));
    }
    // for (auto&& result: results) { result.get(); }

    pool_.enqueue(&DownloadTask::speedAndRemainingTimeCalculate, this);
}

void DownloadTask::stop()
{
    spdlog::info("Download stop");
    std::lock_guard<std::mutex> lg(statsMutex_);
    status_ = Status::STOP;
}

void DownloadTask::pause()
{
    spdlog::info("Download pause");
    std::lock_guard<std::mutex> lg(statsMutex_);
    status_ = Status::PAUSE;
}

void DownloadTask::resume()
{
    spdlog::info("Download resume");
    std::lock_guard<std::mutex> lg(statsMutex_);
    status_ = Status::RUNNING;
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

DownloadItem DownloadTask::downloadInfo()
{
    DownloadItem item;
    item.filename = filename_;
    item.filenamePath = tmpFilenamePath_;
    item.url = url_;
    item.totalBytes = totalSize_;
    item.downloadedBytes = downloadedSize_;

    using ItemStatus = DownloadItem::DownloadStatus;
    switch (status_) {
        case Status::PAUSE:
            item.status = ItemStatus::Pause;
            break;
        case Status::RUNNING:
            item.status = ItemStatus::Downloading;
            break;
        case Status::STOP:
            item.status = ItemStatus::Stop;
            break;
        default:
            break;
    }
    return item;
}

DownloadTask::HeadInfo DownloadTask::requestFileInfoFromHead()
{
    session_.SetHeader(header_);
    // disable ssl verify
    session_.SetVerifySsl(false);

    // Head 404 sometimes, should be get filename by Get
    //see:https://github.com/libcpr/cpr/pull/599
    cpr::Response response = session_.Head();

    if (response.status_code >= 400) {
        spdlog::warn("Request head failed. code:", response.status_code);
    }

    HeadInfo fileInfo;
    fileInfo.length = fileSize(response.header);
    fileInfo.supportRange = isAcceptRange(response.header);
    fileInfo.filename = fileName(response);

    spdlog::info("Request head. Accept-Ranges support:{} file name:{} file length:{}", fileInfo.supportRange,
                 fileInfo.filename, fileInfo.length);
    return fileInfo;
}

unsigned long DownloadTask::fileSize(const cpr::Header& header)
{
    unsigned long length{};

    if (auto search = header.find("Content-Length"); search != header.end()) {
        length = std::stoul(search->second);
        spdlog::info("Get file size {} byte from header['Content-Length']", length);
    }
    if (length <= 0) {
        auto tmpLength = session_.GetDownloadFileLength();
        spdlog::info("Get file size {} byte from GetDownloadFileLength()", tmpLength);

        length = tmpLength < 0 ? 0 : tmpLength;
    }

    return length;
}

bool DownloadTask::isAcceptRange(const cpr::Header& header)
{
    if (auto search = header.find("Accept-Ranges"); search != header.end()) {
        auto acceptRange = search->second;
        // not support Accept-Ranges:
        // haven't Accept-Ranges header or value is none
        if (acceptRange == "bytes") {
            return true;
        }
    }
    return false;
}

std::string DownloadTask::fileName(const cpr::Response& response)
{
    auto headers = response.header;
    auto url = response.url.str();
    // First, try to get it from the Content-Disposition header.
    auto getFilenameFromHeader = [](const cpr::Header& headers) -> std::string {
        if (auto it = headers.find("Content-Disposition"); it != headers.end()) {
            std::string header = it->second;
            // \s*：匹配任意数量的空白字符
            // ["']?：匹配0个或1个双引号或单引号
            // ([^"']*)：匹配任意数量的非双引号和非单引号字符，这是文件名。这是第一个捕获组。
            std::regex filenameRegex(R"(filename\s*=\s*["']?([^"']*)["']?)");
            std::smatch match;
            if (std::regex_search(header, match, filenameRegex)) {
                if (match.size() > 1) {               // 确保匹配到文件名
                    std::string name = match[1].str();// 第一个捕获组是文件名
                    return cpr::util::urlDecode(name);
                }
            }
        }
        return {};
    };

    auto getFilenameFromGet = [this, getFilenameFromHeader]() -> std::string {
        session_.SetRange(cpr::Range{0, 1});
        auto response = session_.Get();
        if (response.status_code == 200 || response.status_code == 206) {
            return getFilenameFromHeader(response.header);
        }
        return {};
    };

    auto getFilenameFromUrl = [&url]() -> std::string {
        // TODO: maybe not correct
        size_t pos = url.find_last_of('/');
        if (pos != std::string::npos) {
            return cpr::util::urlDecode(url.substr(pos + 1));
        }
        return {};
    };

    auto filename = getFilenameFromHeader(headers);
    if (filename.empty()) {
        spdlog::warn("Get filename from header failed, try get it from Get request");
        filename = getFilenameFromGet();
    }
    if (filename.empty()) {
        spdlog::warn("Get filename from Get reqeust failed, try get it from url");
        filename = getFilenameFromUrl();
    }
    if (filename.empty()) {
        spdlog::warn("Get filename from url failed, return default name");
        filename = "downloading";
    }
    spdlog::info("Get filename success. File name:{}", filename);
    return filename;
}

void DownloadTask::preallocateFileSize(uint64_t fileSize)
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

    fs::path filename = tmpFilenamePath_;
    std::ofstream file(filename, std::ios::binary | std::ios::out);
    session_.SetProgressCallback(cpr::ProgressCallback(
            [this](long downloadTotal, long downloadNow, long uploadTotal, long uploadNow, auto userdata) {
                return progressCallback(downloadTotal, downloadNow, uploadTotal, uploadNow, userdata);
            }));
    session_.UpdateHeader(header_);
    // launch monitor thread
    pool_.enqueue(&DownloadTask::speedAndRemainingTimeCalculate, this);

    auto response = session_.Download(file);
    if (response.status_code == 200 || response.downloaded_bytes == totalSize_) {
        fs::path path(tmpFilenamePath_);
        // remove .part suffix
        auto filenamePath = path.replace_extension("").string();
        fs::rename(tmpFilenamePath_, filenamePath);
        if (downloadCompleteCallback_) {
            downloadCompleteCallback_();
        }

        spdlog::info("Download small file finish. file path:{}", filename.string());
    }
    else {
        spdlog::error("Download small file failed. code:{} error reason: {}", response.status_code, response.reason);
    }
    status_ = Status::STOP;
}

void DownloadTask::downloadChunk(int part, uint64_t start, uint64_t end)
{
    if (status_ != Status::RUNNING)
        return;

    cpr::ProgressCallback progressCallbackFunc(
            [this](long downloadTotal, long downloadNow, long uploadTotal, long uploadNow, auto userdata) {
                return progressCallback(downloadTotal, downloadNow, uploadTotal, uploadNow, userdata);
            });

    ChunkFile f;
    f.readLen = 0;
    f.start = start;
    f.end = end;

    cpr::WriteCallback writeCallbackFunc(
            [this](const std::string_view& data, intptr_t userdata) {
                // Return true on success, or false to cancel the transfer.
                if (status_ != Status::RUNNING) {
                    // TODO:return false will cancel download. try to pause
                    return false;
                }
                return writeCallback(data, userdata);
            },
            reinterpret_cast<intptr_t>(&f));

    cpr::Response response = cpr::Get(cpr::Url{url_}, header_, cpr::Range(start, end), cpr::VerifySsl{false},
                                      progressCallbackFunc, writeCallbackFunc);

    if (response.status_code != 206) {
        spdlog::error("Thread failed to download part. Status code: {}", response.status_code);
        return;
    }
    else {
        spdlog::info("Thread download range[{}:{}] file finished.", start, end);

        if (isDownloadComplete()) {
            spdlog::info("All thread download complete");

            status_ = Status::STOP;

            if (downloadCompleteCallback_) {
                downloadCompleteCallback_();
            }
        }
    }
}

bool DownloadTask::writeCallback(const std::string_view& data, intptr_t userdata)
{
    ChunkFile* pf = reinterpret_cast<ChunkFile*>(userdata);
    pf->readLen += data.size();
    pf->data.append(data);

    if (pf->readLen == pf->end - pf->start + 1) {
        try {
            // the destructor will invoke sync() and unmap()
            mio::mmap_sink mmap(tmpFilenamePath_, pf->start);

            std::copy(data.begin(), data.end(), mmap.begin());

            fs::path path(tmpFilenamePath_);
            // remove .part suffix
            auto filenamePath = path.replace_extension("").string();
            fs::rename(tmpFilenamePath_, filenamePath);

            downloadedSize_ += pf->data.size();
        }
        catch (const std::system_error& e) {
            spdlog::error("Write chunk file failed. Exception:{}", e.what());
        }
        spdlog::info("Write chunk file completed. start:{} read len:{} downloaded size:{}", pf->start, pf->readLen,
                     downloadedSize_.load());
    }

    return true;
}

bool DownloadTask::progressCallback(long downloadTotal, long downloadNow, long uploadTotal, long uploadNow,
                                    intptr_t userdata)
{
    (void) uploadTotal;
    (void) uploadNow;
    (void) userdata;

    if (totalSize_ == 0) {
        totalSize_.store(downloadTotal, std::memory_order_release);
    }

    if (progressCallback_ && status_ == Status::RUNNING) {
        progressCallback_(totalSize_.load(std::memory_order_acquire), downloadedSize_.load(std::memory_order_acquire),
                          speed_.load(std::memory_order_acquire), remainTime_.load(std::memory_order_acquire));
    }

    spdlog::debug("Download {}/{} bytes.", downloadNow, downloadTotal);

    return true;
}

void DownloadTask::setProgressCallback(const ProgressCallback& cb)
{
    progressCallback_ = cb;
}

void DownloadTask::setDownloadCompleteCallback(const DownloadCompleteCallback& cb)
{
    downloadCompleteCallback_ = cb;
}

bool DownloadTask::isDownloadComplete()
{
    if (totalSize_ == downloadedSize_)
        return true;
    return false;
}

void DownloadTask::speedAndRemainingTimeCalculate()
{
    using namespace std::chrono_literals;
    while (status_ != Status::STOP) {
        if (status_ == Status::PAUSE) {
            std::this_thread::sleep_for(500ms);
            continue;
        }

        auto previous_size = downloadedSize_.load(std::memory_order_relaxed);
        std::this_thread::sleep_for(1s);
        speed_.store(downloadedSize_.load(std::memory_order_relaxed) - previous_size,
                     std::memory_order_relaxed);// bytes per second
        remainTime_.store((speed_ > 0) ? static_cast<double>(totalSize_ - downloadedSize_) /
                                                 speed_.load(std::memory_order_relaxed)
                                       : 0,
                          std::memory_order_relaxed);
        spdlog::debug("Speed: {}KB/s, Remaining time: {}s", speed_.load() / 1024, remainTime_.load());
    }
}
