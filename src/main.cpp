#include <QApplication>
#include <QDir>
#include <QFile>
#include <QIcon>
#include <SingleApplication>

#include "Logger.hpp"
#include "MainWindow.h"
#include "Path.h"
#include "version.h"
#include "utils/Utils.h"

void loadQssStyle()
{
    // TODO:检测设置或系统颜色主题，应用对应样式表
    QFile qssFile(":/qss/resources/style/style_white.qss");
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
#if _WIN32
    // TODO: 目前为Qt环境，启用可能会导致其他问题
    // 设置标准库调用系统 API 所用的编码，用于 fopen，ifstream 等函数
    // setlocale(LC_ALL, ".UTF-8");
    // 设置控制台输出编码
    SetConsoleOutputCP(CP_UTF8);
    // 设置控制台输入编码，用于 std::cin
    SetConsoleCP(CP_UTF8);
#endif

    SingleApplication a(argc, argv);
    a.setApplicationName("FlowD");

    initLogger();
    loadQssStyle();

#if defined(Q_OS_WIN)
    a.setWindowIcon(QIcon(":/resources/win/logo.ico"));
#elif defined(Q_OS_MAC)
    a.setWindowIcon(QIcon(":/resources/mac/logo.icns"));
#else
    a.setWindowIcon(QIcon(":/resources/logo.png"));
#endif

    spdlog::info("App launched. Version:{}", VERSION_STR);
    spdlog::info("Cache path:{}", utils::Path::instance().cachePath());
    spdlog::info("Current system theme is {}", utils::isSystemThemeDark() ? "dark" : "light");
#ifdef Q_OS_WIN
    spdlog::info("Current Windows system is win10 or greater: {}", utils::isWin11OrGreater());
#endif

    MainWindow w;
    w.setWindowTitle("FlowD " + QString(VERSION_STR));
    w.show();

    QObject::connect(&a, &SingleApplication::instanceStarted, &w, &MainWindow::show);

    return QApplication::exec();
}
