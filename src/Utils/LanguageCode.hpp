//
// Created by Leo on 24-9-15.
//

#pragma once

#include <unordered_map>
#include <string>

static std::unordered_map<std::string, std::string> languageCode{
        {"zh-CN","中文"},
        {"en","English"}
};

static std::string languageMap(const std::string& code)
{
    if (auto search = languageCode.find(code); search != languageCode.end())
    {
        return search->second;
    }
    return {};
}