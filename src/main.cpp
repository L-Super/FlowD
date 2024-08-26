#include <QApplication>
#include <QDebug>
#include <QFile>
#include <QIcon>
#include <QMessageBox>

#include "MainWindow.h"
#include "version.h"

//获取样式表
QString getQss()
{
    //TODO:检测设置或系统颜色主题，应用对应样式表
    QFile qssFile(":/qss/resources/style/style_white.qss");
    QString styleSheet;
    if (qssFile.open(QIODevice::ReadOnly)) {
        styleSheet = QString::fromLatin1(qssFile.readAll());
    }
    else {
        //TODO:写入日志
        QMessageBox::critical(nullptr, "失败", "获取样式表失败");
    }
    return styleSheet;
}

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/resources/logo.png"));

    qDebug() << QString("The version of this application is v%1").arg(VERSION_STR);

    //应用样式表
    a.setStyleSheet(getQss());


    MainWindow w;
    w.setWindowTitle("FlowD " + QString(VERSION_STR));
    w.show();
    return QApplication::exec();
}
