//
// Created by LMR on 24-9-6.
//

#include "AppConfig.h"
#include "Logger.hpp"
#include "Path.h"

#include <filesystem>

namespace {
    const std::string filepath = utils::Path::instance().configFilepath().toStdString();
}// namespace

AppConfig::AppConfig()
{
    load();
}

AppConfig::~AppConfig()
{
    save();
}

void AppConfig::load()
{
    if (!std::filesystem::exists(filepath)) {
        std::ofstream ofs(filepath);
    }
    try {
        std::ifstream f(filepath);
        data_ = json::parse(f);
    }
    catch (const std::exception& e) {
        spdlog::error("Failed to parse json file: {}", e.what());
    }
}

void AppConfig::save()
{
    std::ofstream file(filepath);
    if (!file.is_open()) {
        spdlog::critical("Failed to open file: {}", filepath);
    }
    file << std::setw(2) << data_;
}
