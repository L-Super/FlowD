//
// Created by LMR on 24-9-1.
//

#pragma once

#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include <QByteArray>
#include <QString>
#include <chrono>
#include <iostream>
#include <type_traits>

template<>
struct fmt::formatter<QString> : formatter<string_view> {
    template<typename FormatContext>
    auto format(QString str, FormatContext& ctx)
    {
        return formatter<string_view>::format(str.toStdString(), ctx);
    }
};

template<>
struct fmt::formatter<QByteArray> : formatter<string_view> {
    template<typename FormatContext>
    auto format(QByteArray str, FormatContext& ctx)
    {
        return formatter<string_view>::format(str.toStdString(), ctx);
    }
};

template<typename T>
std::ostream& operator<<(typename std::enable_if<std::is_enum<T>::value, std::ostream>::type& stream, const T& e)
{
    return stream << static_cast<typename std::underlying_type<T>::type>(e);
}

class Log {
public:
    static Log& instance(const std::string& processName, const std::string& filePath)
    {
        static Log log(processName, filePath);
        return log;
    }

    static void setLevel(int level)
    {
        switch (level) {
            case 0:
                spdlog::set_level(spdlog::level::trace);
                break;
            case 1:
                spdlog::set_level(spdlog::level::debug);
                break;
            case 2:
                spdlog::set_level(spdlog::level::info);
                break;
            case 3:
                spdlog::set_level(spdlog::level::warn);
                break;
            case 4:
                spdlog::set_level(spdlog::level::err);
                break;
            case 5:
                spdlog::set_level(spdlog::level::critical);
                break;
            case 6:
                spdlog::set_level(spdlog::level::off);
                break;
            default:
                spdlog::set_level(spdlog::level::info);
        }
    }

private:
    explicit Log(const std::string& name, const std::string& filePath)
    {
        auto maxSize = 1024 * 1024 * 5;// 5MB
        auto maxFiles = 3;

        auto stdout_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        auto rotating_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(filePath, maxSize, maxFiles);
        std::vector<spdlog::sink_ptr> sinks{stdout_sink, rotating_sink};
        auto logger = std::make_shared<spdlog::logger>(name, begin(sinks), end(sinks));

        spdlog::set_default_logger(logger);

#if defined(QT_DEBUG)
        std::string format{R"([%Y-%m-%d %T.%e] [%^%l%$] [%n] [thread %t] %v)"};
#else
        std::string format{R"([%Y-%m-%d %T.%e] [%^%l%$] [%n] %v)"};
#endif
        spdlog::set_pattern(format);
        spdlog::flush_every(std::chrono::seconds(5));
    }

public:
    Log(const Log&) = delete;
    Log& operator=(const Log&) = delete;
    Log(Log&&) = delete;
    Log& operator=(Log&&) = delete;
    ~Log() = default;
};