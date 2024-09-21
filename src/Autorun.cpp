#include "Autorun.h"

#include <QApplication>
#include <QDir>
#include <QProcess>
#include <QSettings>
#include <QStandardPaths>

#include "version.h"

bool AutoRun::setAutoRun()
{
    bool done = false;
#if defined(Q_OS_WIN)
    done = setAutoRunWindows();
#elif defined(Q_OS_MAC)
    done = setAutoRunMac();
#elif defined(Q_OS_LINUX)
    done = setAutoRunLinux();
#else
    //有点抽象
    return 0;
#endif
    return done;
}

bool AutoRun::removeAutoRun()
{
    bool done = false;
#if defined(Q_OS_WIN)
    done = removeAutoRunWindows();
#elif defined(Q_OS_MAC)
    done = removeAutoRunMac();
#elif defined(Q_OS_LINUX)
    done = removeAutoRunLinux();
#else
    return 0;
#endif
    return done;
}

bool AutoRun::setAutoRunLinux()
{
    QString path = QStandardPaths::writableLocation(QStandardPaths::GenericConfigLocation);
    QString autoRunFileName = path + "/autostart/FlowD.desktop";
    QFile autoRunFile(autoRunFileName);
    if (!autoRunFile.open(QIODevice::WriteOnly))
        return 0;
    QTextStream stream(&autoRunFile);
    stream.setAutoDetectUnicode(true);

    stream << "[Desktop Entry]" << QString("Exec=%1 min").arg(QApplication::applicationFilePath())
           << QString("Icon=%1/resource/logo.png").arg(QApplication::applicationDirPath()) << QString("Terminal=false")
           << "Type=Application";

    if (isDeepinSystem()) {
        stream << "X-Deepin-Vendor=user-custom";
    }

    autoRunFile.close();
    return 1;
}

bool AutoRun::setAutoRunWindows()
{
    const QString autoRegPath = "HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run";

    QSettings settings(autoRegPath, QSettings::NativeFormat);
    QString para = QDir::toNativeSeparators(QApplication::applicationFilePath() + " min");
    settings.setValue(QApplication::applicationName(), para);

    //TODO:determine whether the application succeeded
    return 1;
}

bool AutoRun::setAutoRunMac()
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

bool AutoRun::removeAutoRunLinux()
{
    QString path = QStandardPaths::writableLocation(QStandardPaths::GenericConfigLocation);
    QString autoRunFileName = path + "/autostart/FlowD.desktop";
    QFile autoRunFile(autoRunFileName);
    if (!autoRunFile.remove())
        return 0;
    return 1;
}

bool AutoRun::removeAutoRunWindows()
{
    const QString autoRegPath = "HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run";

    QSettings settings(autoRegPath, QSettings::NativeFormat);
    settings.remove(QApplication::applicationName());
    //TODO:determine whether the application succeeded
    return 1;
}

bool AutoRun::removeAutoRunMac()
{
    QString plistPath =
            QDir::cleanPath(QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + QDir::separator() +
                            "Library" + QDir::separator() + "LaunchAgents" + QDir::separator() + "net.my.app.plist");

    // delete .plist File
    QFile file(plistPath);
    return file.remove();
}

bool AutoRun::isDeepinSystem()
{
    QProcess process;
    process.start("env");
    process.waitForFinished();
    QString output = process.readAllStandardOutput();
    return output.contains("DEEPIN_SESSION_ID");
}
