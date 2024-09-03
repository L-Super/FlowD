//
// Created by LMR on 24-9-3.
//

#pragma once
#include <QString>

namespace utils {
    class Path {
    public:
        static QString cachePath();
        static QString logPath();

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
        Path() = default;
        ~Path() = default;
    };
}// namespace utils
