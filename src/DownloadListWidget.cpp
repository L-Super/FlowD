#include "DownloadListWidget.h"
#include "ui_DownloadListWidget.h"

#include <QListWidgetItem>

#include "DownloadItemWidget.h"

DownloadListWidget::DownloadListWidget(QWidget* parent) : QWidget(parent), ui(new Ui::DownloadListWidget)
{
    ui->setupUi(this);

    //仅供测试
    QListWidgetItem* pItem = new QListWidgetItem(ui->listWidget);
    ui->listWidget->addItem(pItem);

    DownloadItemWidget* aCustomItem = new DownloadItemWidget;
    ui->listWidget->setItemWidget(pItem, aCustomItem);
    ui->listWidget->setCurrentItem(pItem);

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
}
