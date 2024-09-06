//
// Created by LMR on 24-9-3.
//

#include "Path.h"
#include <QDir>
#include <QStandardPaths>

namespace utils {
    Path::Path()
    {
        cachePath_ = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
        createDir(cachePath_);
        logPath_ = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation) + "/logs";
        createDir(logPath_);
    }

    const QString& utils::Path::cachePath()
    {
        return cachePath_;
    }

    const QString& Path::logPath()
    {
        return logPath_;
    }

    void Path::createDir(const QString& path)
    {
        QDir dir(path);
        if (!dir.exists()) {
            dir.mkpath(".");
        }
    }
}// namespace utils