#include "DownloadListWidget.h"
#include "ui_DownloadListWidget.h"

#include <QListWidgetItem>

#include "DownloadItemWidget.h"

DownloadListWidget::DownloadListWidget(QWidget* parent) : QWidget(parent), ui(new Ui::DownloadListWidget)
{
    ui->setupUi(this);

    //仅供测试（使用示例）

    listItems.append(new QListWidgetItem);
    itemsHash.insert(listItems[0], new DownloadItemWidget);

    ui->listWidget->addItem(listItems[0]);
    ui->listWidget->setItemWidget(listItems[0], itemsHash[listItems[0]]);

    /*下面示例展示如何获取listItem第一个元素（类型QListItemWidget实例）所对应的

    DownloadItemWidget对象的指针，并进行一些操作：
    DownloadItemWidget* itemWidget=itemsHash[listItems[0]];
    itemWidget->setFileName("FlowD.exe");
    itemWidget->setDownloadState(true);

    这里你不用担心内存泄漏，我已经在析构函数里正确地释放了*/

    iniUi();
    connectSlots();
}

void DownloadListWidget::iniUi()
{
    //初次选中“下载中”
    ui->btnTabDownloading->setChecked(true);
    changeTab(0);
}

void DownloadListWidget::connectSlots()
{
    connect(ui->btnTabDownloading, &QPushButton::clicked, this, [=](bool checked) {
        if (checked)
            changeTab(0);
        else
            ui->btnTabDownloading->setChecked(true);
    });
    connect(ui->btnTabStopped, &QPushButton::clicked, this, [=](bool checked) {
        if (checked)
            changeTab(1);
        else
            ui->btnTabStopped->setChecked(true);
    });
}

void DownloadListWidget::changeTab(int index)
{
    if (index == 0) {
        ui->btnTabStopped->setChecked(false);
        //TODO:过滤下载条目
    }
    else if (index == 1) {
        ui->btnTabDownloading->setChecked(false);
        //TODO:过滤下载条目
    }
}

DownloadListWidget::~DownloadListWidget()
{
    delete ui;

    //释放QListWidgetItem类对象和DownloadItemWidget类对象的所有资源
    QHash<QListWidgetItem*, DownloadItemWidget*>::iterator it = itemsHash.begin();
    while (it != itemsHash.end()) {
        QHash<QListWidgetItem*, DownloadItemWidget*>::iterator current = it;
        ++it;
        delete current.key();
        delete current.value();
        itemsHash.erase(current);
    }
}
