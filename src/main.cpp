#include <QApplication>
#include <QDir>
#include <QFile>
#include <QIcon>
#include <SingleApplication>

#include "Logger.hpp"
#include "MainWindow.h"
#include "Path.h"
#include "version.h"

void loadQssStyle()
{
    // TODO:检测设置或系统颜色主题，应用对应样式表
    QFile qssFile(":/qss/Resources/style/style_white.qss");
    if (qssFile.open(QIODevice::ReadOnly)) {
        qApp->setStyleSheet(qssFile.readAll());
    }
    qssFile.close();
}

void initLogger()
{
    std::string filepath = QString(utils::Path::instance().logPath() + "/flowd.log").toStdString();
    Log::instance("FlowD", filepath);
    Log::setLevel(1);
}

int main(int argc, char* argv[])
{
    SingleApplication a(argc, argv);
    a.setApplicationName("FlowD");

    initLogger();
    loadQssStyle();

#if defined(Q_OS_WIN)
    a.setWindowIcon(QIcon(":/Resources/win/logo.ico"));
#elif defined(Q_OS_MAC)
    a.setWindowIcon(QIcon(":/Resources/mac/logo.icns"));
#else
    a.setWindowIcon(QIcon(":/Resources/logo.png"));
#endif

    spdlog::info("App launched. Version:{}", VERSION_STR);
    spdlog::info("Cache path:{}", utils::Path::instance().cachePath());

    MainWindow w;
    w.setWindowTitle("FlowD " + QString(VERSION_STR));

    //Determine whether the window needs to show

#if defined(Q_OS_WIN)
    QStringList arguments = QCoreApplication::arguments();
    if (arguments.size() > 1) {
        w.hide();
    }
    else {
        w.show();
    }
#endif

    QObject::connect(&a, &SingleApplication::instanceStarted, &w, &MainWindow::show);

    return QApplication::exec();
}
