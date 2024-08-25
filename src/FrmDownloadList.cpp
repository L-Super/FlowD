#include "FrmDownloadList.h"
#include "ui_FrmDownloadList.h"

#include <QListWidgetItem>

#include "FrmDownloadItem.h"

FrmDownloadList::FrmDownloadList(QWidget* parent) : QWidget(parent), ui(new Ui::FrmDownloadList)
{
    ui->setupUi(this);

    //仅供测试
    QListWidgetItem* pItem = new QListWidgetItem(ui->listWidget);
    ui->listWidget->addItem(pItem);

    FrmDownloadItem* aCustomItem = new FrmDownloadItem;
    ui->listWidget->setItemWidget(pItem, aCustomItem);
    ui->listWidget->setCurrentItem(pItem);

    iniUi();
    connectSlots();
}

void FrmDownloadList::iniUi()
{
    //初次选中“下载中”
    ui->btnTabDownloading->setChecked(true);
    changeTab(0);
}

void FrmDownloadList::connectSlots()
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

void FrmDownloadList::changeTab(int index)
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

FrmDownloadList::~FrmDownloadList()
{
    delete ui;
}
