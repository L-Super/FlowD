#pragma once

#include <QObject>

#include <Noncopyable.hpp>

class AutoStartUp : public Noncopyable {
public:
    static bool setAutoStartUp();

    static bool removeAutoStartUp();

    static bool isAutoStartUp();

private:
    static bool setAutoStartUpLinux();
    static bool setAutoStartUpWindows();
    static bool setAutoStartUpMac();

    static bool removeAutoStartUpLinux();
    static bool removeAutoStartUpWindows();
    static bool removeAutoStartUpMac();

    static bool isAutoStartUpLinux();
    static bool isAutoStartUpWindows();
    static bool isAutoStartUpMac();

    static bool isDeepinSystem();
};
