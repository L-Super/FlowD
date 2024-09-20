#include "Autorun.h"

#include <QApplication>
#include <QDir>
#include <QSettings>

AutoRun& AutoRun::instance()
{
    static AutoRun autoRun(QApplication::applicationName(), QApplication::applicationFilePath());
    return autoRun;
}

bool AutoRun::setAutoRun()
{
    bool done = false;
#if defined(Q_OS_WIN)

    const QString autoRegPath = "HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run";

    QSettings settings(autoRegPath, QSettings::NativeFormat);
    QString para = QDir::toNativeSeparators(QApplication::applicationFilePath() + " min");
    settings.setValue(QApplication::applicationName(), para);
    done = true;
#elif defined(Q_OS_MAC)
    //TODO:add autorun for mac
#elif defined(Q_OS_LINUX)
    //TODO:add autorun for linux
#else
    return 0;
#endif
    return done;
}

bool AutoRun::removeAutoRun()
{
    bool done = false;
#if defined(Q_OS_WIN)

    const QString autoRegPath = "HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run";

    QSettings settings(autoRegPath, QSettings::NativeFormat);
    settings.remove(QApplication::applicationName());

    done = true;
#elif defined(Q_OS_MAC)
    //TODO:remove autorun for mac
#elif defined(Q_OS_LINUX)
    //TODO:remove autorun for linux
#else
    return 0;
#endif
    return done;
}

AutoRun::AutoRun(QString appName, QString appFilePath) : appName_(appName), appFilePath_(appFilePath) {}
