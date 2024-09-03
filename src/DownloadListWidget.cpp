#include "DownloadListWidget.h"
#include "ui_DownloadListWidget.h"

#include <QListWidgetItem>

#include "DownloadItemWidget.h"

DownloadListWidget::DownloadListWidget(QWidget* parent) : QWidget(parent), ui(new Ui::DownloadListWidget)
{
    ui->setupUi(this);

    //For testing purposes only (usage example)

    DownloadItemWidget* item = addDownloadItem("http://pinsoftstd.cn/FlowD.exe", "FlowD.exe",
                                               "D:/Users/Administrator/Download/", 10240000, 0, true);

    item->setDownloadSpeed(100000000);
    item->setDownloadState(false);

    connect(item, &DownloadItemWidget::removeFromWidgetRequested, this,
            &DownloadListWidget::onRemoveFromWidgetRequested);

    item = addDownloadItem("http://pinsoftstd.cn/FlowD.exe", "FlowD.exe", "D:/Users/Administrator/Download/", 10240000,
                           0, true);

    item->setDownloadSpeed(100000000);
    item->setDownloadState(false);


    connect(item, &DownloadItemWidget::removeFromWidgetRequested, this,
            &DownloadListWidget::onRemoveFromWidgetRequested);


    iniUi();
    connectSlots();
}

void DownloadListWidget::iniUi()
{
    // Initially select "Downloading"
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
    int nums = ui->listWidget->count();

    if (index == 0) {
        ui->btnTabStopped->setChecked(false);
        //Filter download entries
        for (int i = 0; i < nums; ++i) {
            DownloadItemWidget* itemWidget =
                    qobject_cast<DownloadItemWidget*>(ui->listWidget->itemWidget(ui->listWidget->item(i)));
            if (itemWidget->getDownloadState() == true) {
                //If it is downloading
                ui->listWidget->item(i)->setHidden(0);
            }
            else {
                ui->listWidget->item(i)->setHidden(1);
            }
        }
    }
    else if (index == 1) {
        ui->btnTabDownloading->setChecked(false);
        for (int i = 0; i < nums; ++i) {
            DownloadItemWidget* itemWidget =
                    qobject_cast<DownloadItemWidget*>(ui->listWidget->itemWidget(ui->listWidget->item(i)));
            if (itemWidget->getDownloadState() == false) {
                //If downloading has stopped
                ui->listWidget->item(i)->setHidden(0);
            }
            else {
                ui->listWidget->item(i)->setHidden(1);
            }
        }
    }
}

DownloadItemWidget* DownloadListWidget::addDownloadItem(QString URL, QString fileName, QString fileSavedPath,
                                                        qint64 totalBytes, qint64 downloadedBytes, bool isDownloading)
{
    QListWidgetItem* item = new QListWidgetItem();
    DownloadItemWidget* customItem = new DownloadItemWidget(URL, fileName, fileSavedPath, totalBytes, downloadedBytes,
                                                            isDownloading, item, this);

    item->setSizeHint(QSize(600, 120));

    ui->listWidget->addItem(item);
    ui->listWidget->setItemWidget(item, customItem);
    return customItem;
}

void DownloadListWidget::onRemoveFromWidgetRequested(QListWidgetItem* itemToRemove)
{
    ui->listWidget->removeItemWidget(itemToRemove);
    delete itemToRemove;
}

DownloadListWidget::~DownloadListWidget()
{
    delete ui;
}
