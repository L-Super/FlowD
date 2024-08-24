#include <QApplication>
#include <QDebug>
#include <QIcon>

#include "MainWindow.h"
#include "version.h"

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/resources/logo.svg"));

    qDebug() << QString("The version of this application is v%1").arg(VERSION_STR);
    MainWindow w;
    w.setWindowTitle("QTemplate " + QString(VERSION_STR));
    w.show();
    return QApplication::exec();
}
