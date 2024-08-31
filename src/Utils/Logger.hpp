#pragma once

#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <filesystem>

#include <QString>
#include <QStandardPaths>
#include <QSettings>
#include <QDir>

#include "spdlog/spdlog.h"
#include "spdlog/async.h"
#include "spdlog/sinks/basic_file_sink.h"
namespace stream{

}
class Logger
{
public:
    Logger();

    template<typename... Args>
    static void logError(const std::string& fmt, Args&&... args)
    {
        std::ostringstream oss;
        size_t pos = 0;

        // Handling each parameter using fold expressions.
        ((format_helper(oss, fmt, pos, std::forward<Args>(args))), ...);

        // Handling left strings
        while (pos < fmt.length()) {
            oss << fmt[pos++];
        }


        auto logger = spdlog::basic_logger_mt<spdlog::async_factory>("FlowD", Logger::getWritableFilePathName());
        logger->set_level(spdlog::level::err);
        logger->error(oss.str());

        spdlog::flush_every(std::chrono::seconds(10));
        spdlog::drop("logger");
    }

    template<typename... Args>
    static void logInfo(const std::string& fmt, Args&&... args)
    {
        std::ostringstream oss;
        size_t pos = 0;

        ((format_helper(oss, fmt, pos, std::forward<Args>(args))), ...);

        while (pos < fmt.length()) {
            oss << fmt[pos++];
        }
        auto logger = spdlog::basic_logger_mt<spdlog::async_factory>("FlowD", Logger::getWritableFilePathName());
        logger->set_level(spdlog::level::info);
        logger->info(oss.str());

        spdlog::flush_every(std::chrono::seconds(10));
        spdlog::drop("logger");
    }

    template<typename... Args>
    static void logWarning(const std::string& fmt, Args&&... args)
    {
        std::ostringstream oss;
        size_t pos = 0;

        ((format_helper(oss, fmt, pos, std::forward<Args>(args))), ...);

        while (pos < fmt.length()) {
            oss << fmt[pos++];
        }
        auto logger = spdlog::basic_logger_mt<spdlog::async_factory>("FlowD", Logger::getWritableFilePathName());
        logger->set_level(spdlog::level::warn);
        logger->warn(oss.str());

        spdlog::flush_every(std::chrono::seconds(10));
        spdlog::drop("logger");
    }

    template<typename... Args>
    static void terminalError(const std::string& fmt, Args&&... args)
    {
        std::ostringstream oss;
        size_t pos = 0;

        ((format_helper(oss, fmt, pos, std::forward<Args>(args))), ...);

        while (pos < fmt.length()) {
            oss << fmt[pos++];
        }

        spdlog::error(oss.str());
    }

    template<typename... Args>
    static void terminalInfo(const std::string& fmt, Args&&... args)
    {
        std::ostringstream oss;
        size_t pos = 0;

        ((format_helper(oss, fmt, pos, std::forward<Args>(args))), ...);

        while (pos < fmt.length()) {
            oss << fmt[pos++];
        }
        setlocale(LC_ALL, "zh_CN.utf8");
        spdlog::info(oss.str());
    }

    template<typename... Args>
    static void terminalWarning(const std::string& fmt, Args&&... args)
    {
        std::ostringstream oss;
        size_t pos = 0;

        ((format_helper(oss, fmt, pos, std::forward<Args>(args))), ...);

        while (pos < fmt.length()) {
            oss << fmt[pos++];
        }
        setlocale(LC_ALL, "zh_CN.utf8");
        spdlog::warn(oss.str());
    }



private:
    static std::string getWritableFilePathName()
    {
        //Get File index
        QString cachePath = QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
        QDir cacheDir(cachePath);
        if (!cacheDir.exists())
            cacheDir.mkpath(cachePath);
        QSettings set(cachePath + "/config.ini", QSettings::IniFormat);
        int num  = set.value("Log/CurrentNum", 0).toInt();

        if(!std::filesystem::exists("log/")) std::filesystem::create_directories("log/");
        std::string currentFileName="log_"+std::to_string(num)+".log";

        //Get file size
        long size=getFileSize(currentFileName);
        if(size>4096){
            num++;
            if(num>=2){
                num=0;
            }
            currentFileName="log_"+std::to_string(num)+".log";
            if(std::filesystem::exists("log/"+currentFileName))
                std::filesystem::remove("log/"+currentFileName);
            set.setValue("Log/CurrentNum",num);
        }
        std::string returnPathName="log/"+currentFileName;
        return returnPathName;
    }
    static long getFileSize(std::string fileName)
    {
        std::ifstream file("log/"+fileName,std::ifstream::binary);
        if(!file){
            //TODO:Logic of file opening error
            return -1;
        }
        file.seekg(0,file.end);
        long size=file.tellg();
        return size;
    }

    template<typename T>
    static void format_helper(std::ostringstream& oss, const std::string& fmt, size_t& pos, T&& arg) {
        size_t len = fmt.length();
        while (pos < len) {
            if (fmt[pos] == '{' && pos + 1 < len && fmt[pos + 1] == '}') {
                oss << std::forward<T>(arg);
                pos += 2;
                return;
            } else {
                oss << fmt[pos++];
            }
        }
    }
};
