#pragma once

#include <QObject>

#include <Noncopyable.hpp>

class AutoRun : public Noncopyable {
public:
    static AutoRun& instance();

    static bool setAutoRun();

    static bool removeAutoRun();

private:
    AutoRun(QString appName, QString appFilePath);

    QString appName_;
    QString appFilePath_;
};
