#include "DownloadListWidget.h"
#include "ui_DownloadListWidget.h"

#include <QListWidgetItem>

#include "DownloadItemWidget.h"

DownloadListWidget::DownloadListWidget(QWidget* parent) : QWidget(parent), ui(new Ui::DownloadListWidget)
{
    ui->setupUi(this);

    //For testing purposes only (usage example)

    int index = addDownloadItem("http://pinsoftstd.cn/FlowD.deb", "FlowD.deb", "./Download/", 102400000, 0, true);
    /* The following example demonstrates how to create a new item and obtain a pointer
     *  to a DownloadItemWidget object corresponding to the item and perform some operations on it:

    int index=addDownloadItem("http://pinsoftstd.cn/FlowD.exe","FlowD.exe","./Download/",
                                10240000,0,true);
    itemsHash[listItems[item]].setDownloadProgress(1000,12);

    */
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
            DownloadItemWidget* itemWidget = itemsHash[listItems[i]];
            if (itemWidget->getDownloadState() == true) {
                //If it is downloading
                itemWidget->show();
            }
            else {
                itemWidget->hide();
            }
        }
    }
    else if (index == 1) {
        ui->btnTabDownloading->setChecked(false);
        for (int i = 0; i < nums; ++i) {
            DownloadItemWidget* itemWidget = itemsHash[listItems[i]];
            if (itemWidget->getDownloadState() == false) {
                //If downloading has stopped
                itemWidget->show();
            }
            else {
                itemWidget->hide();
            }
        }
    }
}

int DownloadListWidget::addDownloadItem(QString URL, QString fileName, QString fileSavedPath, qint64 totalBytes,
                                        qint64 downloadedBytes, bool isDownloading)
{
    listItems.append(new QListWidgetItem);
    int itemIndex = listItems.count() - 1;
    itemsHash.insert(listItems[itemIndex],
                     new DownloadItemWidget(URL, fileName, fileSavedPath, totalBytes, downloadedBytes, isDownloading));
    /*You don't need to worry about memory leaks here,
    as I have properly handled the deallocation in the destructor. */
    ui->listWidget->addItem(listItems[itemIndex]);
    ui->listWidget->setItemWidget(listItems[itemIndex], itemsHash[listItems[itemIndex]]);
    return itemIndex;
}

DownloadListWidget::~DownloadListWidget()
{
    delete ui;

    // Release all resources associated with QListWidgetItem objects and DownloadItemWidget objects
    QHash<QListWidgetItem*, DownloadItemWidget*>::iterator it = itemsHash.begin();
    while (it != itemsHash.end()) {
        QHash<QListWidgetItem*, DownloadItemWidget*>::iterator current = it;
        ++it;
        delete current.key();
        delete current.value();
        itemsHash.erase(current);
    }
}
