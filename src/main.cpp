#include <QApplication>
#include <QBuffer>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QIcon>
#include <QMessageBox>
#include <QPalette>
#include <QSettings>
#include <QSharedMemory>
#include <QStandardPaths>
#include <QTimer>

#include "SingleApplication"

#include "Logger.hpp"

#include "MainWindow.h"
#include "version.h"

//Get StyleSheet String
QString getQss(SingleApplication& a)
{
    QString cachePath = QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
    QDir cacheDir(cachePath);
    if (!cacheDir.exists())
        cacheDir.mkpath(cachePath);
    QSettings set(cachePath + "/config.ini", QSettings::IniFormat);
    int index = set.value("Basic/DisplayStyle", 0).toInt();
    QFile qssFile;
    QPalette systemPalette = a.palette();
    QColor color = systemPalette.color(QPalette::Window);
    switch (index) {
        case 0:                           //Follow System Theme
            if (color.lightness() < 128) {//Determine System Color Theme
                qssFile.setFileName(":/qss/Resources/style/style_black.qss");
            }
            else {
                qssFile.setFileName(":/qss/Resources/style/style_white.qss");
            }
            break;
        case 1://Light Theme
            qssFile.setFileName(":/qss/Resources/style/style_white.qss");
            break;
        case 2://Dark Theme
            qssFile.setFileName(":/qss/Resources/style/style_black.qss");
            break;
        default:
            set.setValue("Basic/DisplayStyle", 0);
            return getQss(a);
            break;
    }
    QString styleSheet;
    if (qssFile.open(QIODevice::ReadOnly)) {
        styleSheet = QString::fromLatin1(qssFile.readAll());
    }
    else {
        ERROR("Failed in getting stylesheet!");
        QMessageBox::critical(nullptr, QObject::tr("Error"), QObject::tr("Failed in getting style sheet!"));
    }
    qssFile.close();
    return styleSheet;
}

void initLogger()
{
    QString cachePath = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation) + "/logs";
    QDir cacheDir(cachePath);
    if (!cacheDir.exists())
        cacheDir.mkpath(cachePath);

    std::string filepath = cachePath.toStdString() + "/flowd.log";
    Log::instance("FlowD", filepath);
    Log::setLevel(1);
}

int main(int argc, char* argv[])
{
    SingleApplication a(argc, argv);
    // QApplication a(argc, argv);
    initLogger();

    QString iconPath;//icon path

#if defined(Q_OS_WIN)
    iconPath = ":/Resources/win/logo.ico";
#elif defined(Q_OS_MAC)
    iconPath = ":/Resources/mac/logo.icns";
#else//linux or other OS
    iconPath = ":/Resources/logo.png";
#endif

    a.setWindowIcon(QIcon(iconPath));

    QApplication::setQuitOnLastWindowClosed(false);

    qDebug() << QString("The version of this application is v%1").arg(VERSION_STR);


    a.setStyleSheet(getQss(a));

    MainWindow w;
    w.setWindowTitle("FlowD " + QString(VERSION_STR));
    w.show();

    QObject::connect(&a, &SingleApplication::instanceStarted, &w, &MainWindow::show);

    INFO("Application started,Version {}.", VERSION_STR);
    return QApplication::exec();
}
