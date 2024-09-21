#pragma once

#include <QObject>

#include <Noncopyable.hpp>

class AutoRun : public Noncopyable {
public:
    static bool setAutoRun();

    static bool removeAutoRun();

private:
    static bool setAutoRunLinux();
    static bool setAutoRunWindows();
    static bool setAutoRunMac();

    static bool removeAutoRunLinux();
    static bool removeAutoRunWindows();
    static bool removeAutoRunMac();

    static bool isDeepinSystem();
};
