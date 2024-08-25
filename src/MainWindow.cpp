#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QDebug>
#include <QSettings>
#include <QStandardPaths>

MainWindow::MainWindow(QWidget* parent) : QWidget(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connectSlots();
    //千万不能与上一行代码对调，否则会出现野指针
    iniUi();


}


void MainWindow::connectSlots()
{
    //将侧边栏选项卡按钮指针添加至checkableToolButtons，方便后续控制
    checkableToolButtons.append(ui->toolDownloadList);
    checkableToolButtons.append(ui->toolSettings);
    checkableToolButtons.append(ui->toolHelp);

    for(int i=0;i<checkableToolButtons.count();++i)
        connect(checkableToolButtons[i],&QToolButton::clicked,this
                ,&MainWindow::onCheckableToolButtonsClicked);

    //连接其他控件信号和槽
    connect(ui->toolMenu,&QToolButton::clicked,this,&MainWindow::onToolMenuClicked);

}

void MainWindow::iniUi()
{
    //设置选项卡显示状态
    QString cachePath=
            QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
    QSettings set(cachePath+"config.ini");
    tabMinimized=set.value("Common/MinimizeMainTab",false).toBool();
    minimizeMainTab(tabMinimized);


}

//此函数设置主选项卡显示方式是仅有图标还是有图标和文字
void MainWindow::minimizeMainTab(bool minimize)
{
    if(minimize){
        for(int i=0;i<checkableToolButtons.length();++i){
            checkableToolButtons[i]->setToolButtonStyle(Qt::ToolButtonIconOnly);
            checkableToolButtons[i]->setFixedWidth(40);
        }
        ui->toolNewDownload->setToolButtonStyle(Qt::ToolButtonIconOnly);
        ui->toolNewDownload->setFixedWidth(40);
    }else{
        for(int i=0;i<checkableToolButtons.length();++i){
            checkableToolButtons[i]->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
            checkableToolButtons[i]->setFixedWidth(200);
        }
        ui->toolNewDownload->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        ui->toolNewDownload->setFixedWidth(200);
    }

    //将配置写入ini
    QString cachePath=
            QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
    QSettings set(cachePath+"config.ini");
    set.setValue("Common/MinimizeMainTab",minimize);

}

void MainWindow::onCheckableToolButtonsClicked(bool checked)
{
    QToolButton *clickedToolButton=qobject_cast<QToolButton*>(sender());

    //寻找按下的按钮
    for(int i=0;i<checkableToolButtons.count();++i){
        if(clickedToolButton==checkableToolButtons[i]){
            //如果多次点击所在的选项卡，设置属性仍为选中
            if(!checked){
                clickedToolButton->setChecked(true);
            }
            //ui->stackedWidget->setCurrentIndex(i);
        }else{
            //未选中选项卡的则设置未选中
            checkableToolButtons[i]->setChecked(false);
        }
    }
}

void MainWindow::onToolMenuClicked()
{
    tabMinimized=!tabMinimized; //切换状态
    minimizeMainTab(tabMinimized);
}

MainWindow::~MainWindow()
{
    delete ui;
}
