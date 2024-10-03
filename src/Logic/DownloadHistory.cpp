//
// Created by Leo on 24-10-2.
//

#include "DownloadHistory.h"

#include <QFile>
#include <QSqlError>
#include <QSqlQuery>

#include "Logger.hpp"
#include "Path.h"

DownloadHistory::DownloadHistory()
{
    if (!QFile::exists(utils::Path::instance().sqlFilePath())) {
        createDB();
    }
    else {
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName(utils::Path::instance().sqlFilePath());
        if (!db.open()) {
            spdlog::error("Could not connect to database. Error:{}", db.lastError().text());
            return;
        }
        db_ = QSqlDatabase::database();
    }
}

DownloadHistory::~DownloadHistory()
{
    db_.close();
}

bool DownloadHistory::createDB()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(utils::Path::instance().sqlFilePath());
    if (!db.open()) {
        spdlog::error("Could not connect to database. Error:{}", db.lastError().text());
        return false;
    }
    db_ = QSqlDatabase::database();

    // 创建下载任务表
    QString createDownloadsTable = R"(
        CREATE TABLE IF NOT EXISTS downloads (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            url TEXT NOT NULL UNIQUE,          -- 下载文件的 URL
            file_name TEXT,                    -- 下载的文件名
            file_path TEXT,                    -- 文件路径
            total_size INTEGER,                -- 文件的总大小
            thread_count INTEGER,              -- 下载任务的线程数
            status TEXT,                       -- 下载状态: "completed", "downloading", "paused"
            md5 TEXT,                          -- 文件MD5
            timestamp DATETIME DEFAULT CURRENT_TIMESTAMP -- 下载的时间戳
        );
    )";

    QSqlQuery query;
    if (!query.exec(createDownloadsTable)) {
        spdlog::error("Failed to create table 'downloads'. Error:{}", query.lastError().text());
        return false;
    }

    // 创建分段表
    QString createSegmentsTable = R"(
        CREATE TABLE IF NOT EXISTS download_segments (
            segment_id INTEGER PRIMARY KEY AUTOINCREMENT,
            download_url TEXT NOT NULL,        -- 对应下载任务的 URL (downloads 表中的 url)
            range_start INTEGER,               -- 分段的起始字节
            range_end INTEGER,                 -- 分段的结束字节
            downloaded_size INTEGER,           -- 当前已下载的字节数
            is_completed INTEGER DEFAULT 0,    -- 是否下载完成 (0 未完成，1 完成)
            FOREIGN KEY(download_url) REFERENCES downloads(url)
        );
    )";

    if (!query.exec(createSegmentsTable)) {
        spdlog::error("Failed to create table 'download_segments'. Error:{}", query.lastError().text());
        return false;
    }

    spdlog::info("Database and tables created successfully");
    return true;
}

bool DownloadHistory::insertDownloadTask(const QString& url, const QString& fileName, const QString& filePath,
                                         int64_t totalSize, int threadCount)
{
    QSqlQuery query;
    query.prepare(R"(
        INSERT INTO downloads (url, file_name, file_path, total_size, thread_count, status)
        VALUES (?, ?, ?, ?, ?, 'downloading');
    )");

    query.addBindValue(url);
    query.addBindValue(fileName);
    query.addBindValue(filePath);
    query.addBindValue(totalSize);
    query.addBindValue(threadCount);

    if (!query.exec()) {
        spdlog::error("Failed to insert download task. {}. Url:{}", query.lastError().text(), url);
        return false;
    }

    spdlog::info("Download task inserted successfully. Url:{}", url);
    return true;
}

bool DownloadHistory::insertDownloadSegment(const QString& url, int64_t rangeStart, int64_t rangeEnd,
                                            int64_t downloadedSize)
{
    QSqlQuery query;
    query.prepare(R"(
        INSERT INTO download_segments (download_url, range_start, range_end, downloaded_size)
        VALUES (?, ?, ?, ?);
    )");

    query.addBindValue(url);
    query.addBindValue(rangeStart);
    query.addBindValue(rangeEnd);
    query.addBindValue(downloadedSize);

    if (!query.exec()) {
        spdlog::error("Failed to insert download segment. {}. Url:{}", query.lastError().text(), url);
        return false;
    }

    spdlog::info("Download segment inserted successfully. Url:{}", url);
    return true;
}

QVector<DownloadItem> DownloadHistory::getIncompleteDownloadTasks()
{
    QVector<DownloadItem> incompleteTasks;
    QSqlQuery query;

    // 查询未完成的下载任务和已下载的总字节数
    query.prepare(R"(
        SELECT d.file_name, d.file_path, d.url, d.total_size,
               SUM(ds.downloaded_size) as downloadedBytes
        FROM downloads d
        LEFT JOIN download_segments ds
        ON d.url = ds.download_url
        WHERE d.status = 'downloading'
        GROUP BY d.url;
    )");

    if (query.exec()) {
        while (query.next()) {
            DownloadItem item;
            item.filename = query.value("file_name").toString().toStdString();
            item.filenamePath = query.value("file_path").toString().toStdString();
            item.url = query.value("url").toString().toStdString();
            item.totalBytes = query.value("total_size").toLongLong();
            item.downloadedBytes = query.value("downloadedBytes").toLongLong();
            item.status = DownloadItem::DownloadStatus::Pause;
            incompleteTasks.push_back(item);
        }
    }
    else {
        spdlog::error("Failed to retrieve incomplete tasks. {}", query.lastError().text());
    }

    return incompleteTasks;
}

QVector<DownloadItem> DownloadHistory::getCompleteDownloadTasks()
{
    QVector<DownloadItem> completeTasks;
    QSqlQuery query;
    // 查询已完成的下载任务
    query.prepare(R"(
        SELECT d.file_name, d.file_path, d.url, d.total_size
        FROM downloads d
        WHERE d.status = 'completed'
    )");

    if (query.exec()) {
        while (query.next()) {
            DownloadItem item;
            item.filename = query.value("file_name").toString().toStdString();
            item.filenamePath = query.value("file_path").toString().toStdString();
            item.url = query.value("url").toString().toStdString();
            item.totalBytes = query.value("total_size").toLongLong();
            item.downloadedBytes = item.totalBytes;
            item.status = DownloadItem::DownloadStatus::Stop;
            completeTasks.push_back(item);
        }
    }
    else {
        spdlog::error("Failed to retrieve incomplete tasks. {}", query.lastError().text());
    }

    return completeTasks;
}

QList<QPair<int, int>> DownloadHistory::getDownloadSegments(const QString& url)
{
    QList<QPair<int, int>> segments;
    QSqlQuery query;
    query.prepare("SELECT range_start, range_end FROM download_segments WHERE download_url = ? AND is_completed = 0");
    query.addBindValue(url);

    if (query.exec()) {
        while (query.next()) {
            int rangeStart = query.value(0).toInt();
            int rangeEnd = query.value(1).toInt();
            segments.append(qMakePair(rangeStart, rangeEnd));
        }
    }
    else {
        spdlog::error("Failed to retrieve segments. {}", query.lastError().text());
    }

    return segments;
}

bool DownloadHistory::updateSegmentProgress(const QString& url, int64_t rangeStart, int64_t downloadedSize,
                                            bool isCompleted)
{
    QSqlQuery query;
    query.prepare(R"(
        UPDATE download_segments
        SET downloaded_size = ?, is_completed = ?
        WHERE download_url = ? AND range_start = ?;
    )");

    query.addBindValue(downloadedSize);
    query.addBindValue(isCompleted ? 1 : 0);
    query.addBindValue(url);
    query.addBindValue(rangeStart);

    if (!query.exec()) {
        spdlog::error("Failed to update segment progress. {}", query.lastError().text());
        return false;
    }

    spdlog::info("Segment progress updated successfully");
    return true;
}

bool DownloadHistory::deleteDownloadTask(const QString& url)
{
    QSqlQuery query;

    // 先删除分段信息
    query.prepare("DELETE FROM download_segments WHERE download_url = ?");
    query.addBindValue(url);

    if (!query.exec()) {
        spdlog::error("Failed to delete download segments. {}", query.lastError().text());
        return false;
    }

    // 删除任务信息
    query.prepare("DELETE FROM downloads WHERE url = ?");
    query.addBindValue(url);

    if (!query.exec()) {
        spdlog::error("Failed to delete download task. {}", query.lastError().text());
        return false;
    }

    spdlog::info("Download task and segments deleted successfully");
    return true;
}