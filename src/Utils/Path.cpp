//
// Created by LMR on 24-9-3.
//

#include "Path.h"
#include <QDir>
#include <QStandardPaths>

namespace utils {
    QString utils::Path::cachePath()
    {
        auto cachePath = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
        QDir dir(cachePath);
        if (!dir.exists()) {
            dir.mkdir(cachePath);
        }
        return cachePath;
    }

    QString Path::logPath()
    {
        auto logPath = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation) + "/logs";
        QDir dir(logPath);
        if (!dir.exists()) {
            dir.mkdir(logPath);
        }
        return logPath;
    }

}// namespace utils