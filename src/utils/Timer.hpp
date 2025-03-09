//
// Created by LMR on 25-3-9.
//

#pragma once

#include <chrono>
#include <format>
#include <iostream>
#include <source_location>
#include <string>

namespace utils {
/**
 * @brief RAII计时器
 */
class Timer {
public:
    explicit Timer(std::string tag = {}, const std::source_location& location = std::source_location::current())
        : tag_(std::move(tag)), location_(location), start_(std::chrono::high_resolution_clock::now())
    {}

    ~Timer()
    {
        stop();
    }

    void start(std::string tag = {}, const std::source_location& location = std::source_location::current())
    {
        tag_ = std::move(tag);
        location_ = location;
        start_ = std::chrono::high_resolution_clock::now();
    }

    void stop()
    {
        auto end = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start_).count();

        if (tag_.empty())
            std::cout << std::format("[{}:{} - {}] time coast: {} ms ", location_.file_name(), location_.line(),
                                     location_.function_name(), elapsed)
                      << std::endl;
        else
            std::cout << std::format("[{}:{} - {}] tag: {} time coast: {} ms ", location_.file_name(), location_.line(),
                                     location_.function_name(), tag_, elapsed)
                      << std::endl;
    }

    Timer(const Timer&) = delete;
    Timer& operator=(const Timer&) = delete;
    Timer(Timer&&) = delete;
    Timer& operator=(Timer&&) = delete;

private:
    std::string tag_;
    std::source_location location_;
    std::chrono::time_point<std::chrono::high_resolution_clock> start_;
};

}// namespace utils
