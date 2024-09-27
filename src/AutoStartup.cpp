#include "AutoStartup.h"

#include <QApplication>
#include <QDir>
#include <QFile>
#include <QProcess>
#include <QSettings>
#include <QStandardPaths>
#include <QTextStream>

#include "version.h"

bool AutoStartUp::setAutoStartUp()
{
    bool done = false;
#if defined(Q_OS_WIN)
    done = setAutoStartUpWindows();
#elif defined(Q_OS_MAC)
    done = setAutoStartUpMac();
#elif defined(Q_OS_LINUX)
    done = setAutoStartUpLinux();
#else
    //有点抽象
    return 0;
#endif
    return done;
}

bool AutoStartUp::removeAutoStartUp()
{
    bool done = false;
#if defined(Q_OS_WIN)
    done = removeAutoStartUpWindows();
#elif defined(Q_OS_MAC)
    done = removeAutoStartUpMac();
#elif defined(Q_OS_LINUX)
    done = removeAutoStartUpLinux();
#else
    return 0;
#endif
    return done;
}

bool AutoStartUp::setAutoStartUpLinux()
{
    QString path = QStandardPaths::writableLocation(QStandardPaths::GenericConfigLocation);
    QString autoStartUpFileName = path + "/autostart/FlowD.desktop";
    QFile autoStartUpFile(autoStartUpFileName);
    if (!autoStartUpFile.open(QIODevice::WriteOnly))
        return false;
    QTextStream stream(&autoStartUpFile);
    stream.setAutoDetectUnicode(true);

    stream << "[Desktop Entry]" << QString("Exec=%1 min").arg(QApplication::applicationFilePath())
           << QString("Icon=%1/resource/logo.png").arg(QApplication::applicationDirPath()) << QString("Terminal=false")
           << "Type=Application";

    if (isDeepinSystem()) {
        stream << "X-Deepin-CreatedBy=com.deepin.SessionManager";
    }

    autoStartUpFile.close();
    return true;
}

bool AutoStartUp::setAutoStartUpWindows()
{
    const QString autoRegPath = "HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run";

    QSettings settings(autoRegPath, QSettings::NativeFormat);
    QString para = QDir::toNativeSeparators(QApplication::applicationFilePath() + " min");
    settings.setValue(QApplication::applicationName(), para);

    //TODO:determine whether the application succeeded
    return true;
}

bool AutoStartUp::setAutoStartUpMac()
{
    QString plistPath =
            QDir::cleanPath(QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + QDir::separator() +
                            "Library" + QDir::separator() + "LaunchAgents" + QDir::separator() + "FlowD.plist");

    //plist file content
    QString plistContent = R"(  
    <?xml version=\"1.0\" encoding=\"UTF-8\"?>  
    <!DOCTYPE plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN\" \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">  
    <plist version=\"1.0\">  
    <dict>  
        <key>Label</key>  
        <string>net.my.app</string>  
        <key>ProgramArguments</key>  
        <array>  
            <string>" + QCoreApplication::applicationFilePath() + R"("</string>  
        </array>  
        <key>RunAtLoad</key>  
        <true/>  
        <key>WorkingDirectory</key>  
        <string>" + QDir::cleanPath(QCoreApplication::applicationDirPath()) + R"("</string>  
    </dict>  
    </plist>  
    )";

    // Write .plist File
    QFile file(plistPath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << plistContent;
        file.close();
        return true;
    }
    return false;
}

bool AutoStartUp::removeAutoStartUpLinux()
{
    QString path = QStandardPaths::writableLocation(QStandardPaths::GenericConfigLocation);
    QString autoStartUpFileName = path + "/autostart/FlowD.desktop";
    QFile autoStartUpFile(autoStartUpFileName);
    if (!autoStartUpFile.remove())
        return false;
    return true;
}

bool AutoStartUp::removeAutoStartUpWindows()
{
    const QString autoRegPath = "HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run";

    QSettings settings(autoRegPath, QSettings::NativeFormat);
    settings.remove(QApplication::applicationName());
    //TODO:determine whether the application succeeded
    return true;
}

bool AutoStartUp::removeAutoStartUpMac()
{
    QString plistPath =
            QDir::cleanPath(QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + QDir::separator() +
                            "Library" + QDir::separator() + "LaunchAgents" + QDir::separator() + "net.my.app.plist");

    // delete .plist File
    QFile file(plistPath);
    return file.remove();
}

bool AutoStartUp::isDeepinSystem()
{
    QFile file("/etc/os-release");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        while (!in.atEnd()) {
            QString line = in.readLine();
            if (line.contains("Deepin", Qt::CaseInsensitive)) {
                return true;
            }
        }
    }
    return false;
}
