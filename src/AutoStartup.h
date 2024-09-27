#pragma once

#include <QObject>

#include <Noncopyable.hpp>

class AutoStartUp : public Noncopyable {
public:
    static bool setAutoStartUp();

    static bool removeAutoStartUp();

private:
    static bool setAutoStartUpLinux();
    static bool setAutoStartUpWindows();
    static bool setAutoStartUpMac();

    static bool removeAutoStartUpLinux();
    static bool removeAutoStartUpWindows();
    static bool removeAutoStartUpMac();

    static bool isDeepinSystem();
};
