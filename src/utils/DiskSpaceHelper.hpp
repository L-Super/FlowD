//
// Created by Leo on 24-8-28.
//

#pragma once
#include <QStorageInfo>
#include <QString>

namespace utils {
static constexpr auto MB = 1024 * 1024;
static constexpr auto GB = 1024 * 1024 * 1024;

/**
 * Show the total space of the specified disk.
 * @param path specified path
 * @return size
 */
static qint64 totalDiskSpace(const QString& path)
{
    if (path.isEmpty())
        return 0;
    QList<QStorageInfo> storageInfoList = QStorageInfo::mountedVolumes();
    foreach (QStorageInfo storage, storageInfoList) {
        if (storage.isValid() && storage.isReady()) {
            if (path.startsWith(storage.rootPath())) {
                return storage.bytesTotal();
            }
        }
    }
    return 0;
}

/**
 * Show the available space of the specified disk.
 * @param path specified path
 * @return size
 */
static qint64 availableDiskSpace(const QString& path)
{
    if (path.isEmpty())
        return 0;
    QList<QStorageInfo> storageInfoList = QStorageInfo::mountedVolumes();
    foreach (QStorageInfo storage, storageInfoList) {
        if (storage.isValid() && storage.isReady()) {
            if (path.startsWith(storage.rootPath())) {
                return storage.bytesAvailable();
            }
        }
    }
    return 0;
}

static qint64 totalDiskSpaceByGB(const QString& path)
{
    if (path.isEmpty())
        return 0;
    QList<QStorageInfo> storageInfoList = QStorageInfo::mountedVolumes();
    foreach (QStorageInfo storage, storageInfoList) {
        if (storage.isValid() && storage.isReady()) {
            if (path.startsWith(storage.rootPath())) {
                return storage.bytesTotal() / GB;
            }
        }
    }
    return 0;
}

static qint64 availableDiskSpaceByGB(const QString& path)
{
    if (path.isEmpty())
        return 0;
    QList<QStorageInfo> storageInfoList = QStorageInfo::mountedVolumes();
    foreach (QStorageInfo storage, storageInfoList) {
        if (storage.isValid() && storage.isReady()) {
            if (path.startsWith(storage.rootPath())) {
                return storage.bytesAvailable() / GB;
            }
        }
    }
    return 0;
}
}// namespace utils