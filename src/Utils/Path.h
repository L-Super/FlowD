//
// Created by LMR on 24-9-3.
//

#pragma once
#include <QString>

namespace utils {
    class Path {
    public:
        const QString& cachePath();
        const QString& logPath();

    public:
        static Path& instance()
        {
            static Path p;
            return p;
        }
        Path(const Path&) = delete;
        Path& operator=(const Path&) = delete;
        Path(Path&&) = delete;
        Path& operator=(Path&&) = delete;

    private:
        Path();
        ~Path() = default;
        void createDir(const QString& dir);

    private:
        QString cachePath_;
        QString logPath_;
    };
}// namespace utils
