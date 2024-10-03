//
// Created by Leo on 24-10-2.
//

#pragma once
#include <QSqlDatabase>
#include <QString>

#include "DownloadItem.h"

class DownloadHistory {
public:
    /**
     * 插入下载任务
     * @param url
     * @param fileName
     * @param totalSize
     * @param threadCount
     * @return
     */
    bool insertDownloadTask(const QString& url, const QString& fileName, const QString& filePath, int64_t totalSize,
                            int threadCount);
    /**
     * 插入下载分段
     * @param url
     * @param rangeStart
     * @param rangeEnd
     * @param downloadedSize
     * @return
     */
    bool insertDownloadSegment(const QString& url, int64_t rangeStart, int64_t rangeEnd, int64_t downloadedSize);
    /**
     * 查询未完成的下载任务
     * @return
     */
    QVector<DownloadItem> getIncompleteDownloadTasks();
    /**
     * 查询未完成的下载任务
     * @return
     */
    QVector<DownloadItem>  getCompleteDownloadTasks();
    /**
     * 查询某个下载任务的分段信息
     * @param url
     * @return
     */
    QList<QPair<int, int>> getDownloadSegments(const QString& url);
    /**
     * 更新分段下载进度
     * @param url
     * @param rangeStart
     * @param downloadedSize
     * @param isCompleted
     * @return
     */
    bool updateSegmentProgress(const QString& url, int64_t rangeStart, int64_t downloadedSize, bool isCompleted);
    /**
     * 删除下载任务
     * @param url
     * @return
     */
    bool deleteDownloadTask(const QString& url);

protected:
    bool createDB();

public:
    static DownloadHistory& instance()
    {
        static DownloadHistory d;
        return d;
    }

private:
    DownloadHistory();
    ~DownloadHistory();
private:
    QSqlDatabase db_;
};
