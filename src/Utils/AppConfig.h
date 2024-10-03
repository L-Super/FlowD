//
// Created by LMR on 24-9-6.
//

#pragma once

#include "nlohmann/json.hpp"
#include <string>
#include <optional>

class AppConfig {
public:
    static AppConfig& instance()
    {
        static AppConfig a;
        return a;
    }
public:
    using json = nlohmann::json;
    template<typename T>
    void set(const std::string& key, const T& value)
    {
        data_[key] = value;
        save();
    }

    template<typename T>
    std::optional<T> get(const std::string& key)
    {
        if (data_.find(key) != data_.end()) {
            return data_[key].get<T>();
        }
        return std::nullopt;
    }

    template<typename T>
    void setBasic(const std::string& key, const T& value)
    {
        data_["basic"][key] = value;
        save();
    }

    template<typename T>
    std::optional<T> getBasic(const std::string& key)
    {
        if (data_.contains("basic") && data_["basic"].contains(key))
        {
            return data_["basic"][key].get<T>();
        }
        return std::nullopt;
    }

    template<typename T>
    void setAdvance(const std::string& key, const T& value)
    {
        data_["advance"][key] = value;
        save();
    }

    template<typename T>
    std::optional<T> getAdvance(const std::string& key)
    {
        if (data_.contains("advance") && data_["advance"].contains(key))
        {
            return data_["advance"][key].get<T>();
        }
        return std::nullopt;
    }

protected:
    AppConfig();
    ~AppConfig();
    // load a config file
    void load();
    // save a config file
    void save();

private:
    std::string filepath;
    json data_;
};