//
// Created by LMR on 24-8-25.
//

#pragma once
#include <map>
#include <string>

class DownloadTask;
class DownloadManager {
public:
    DownloadManager();
    ~DownloadManager();

private:
    std::map<std::string, DownloadTask*> downloadMap;
};
