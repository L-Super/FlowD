#include "DownloadListWidget.h"
#include "ui_DownloadListWidget.h"

#include <QListWidgetItem>

#include "DownloadItemWidget.h"

DownloadListWidget::DownloadListWidget(QWidget* parent) : QWidget(parent), ui(new Ui::DownloadListWidget)
{
    ui->setupUi(this);

    ui->tabWidget->setCurrentIndex(0);
}

DownloadListWidget::~DownloadListWidget()
{
    delete ui;
}

void DownloadListWidget::addDownloadingItem(DownloadItemWidget* item)
{
    auto widgetItem = new QListWidgetItem();
    ui->downloadingListWidget->addItem(widgetItem);
    ui->downloadingListWidget->setItemWidget(widgetItem, item);

    connect(item, &DownloadItemWidget::completeDownloadSignal, [this, widgetItem, item]() {
        ui->finishedListWidget->addItem(widgetItem);
        ui->finishedListWidget->setItemWidget(widgetItem, item);
        ui->downloadingListWidget->removeItemWidget(widgetItem);
    });
    connect(item, &DownloadItemWidget::removeItemSignal, [this, widgetItem]() {
        auto w = ui->downloadingListWidget->itemWidget(widgetItem);
        if (w != nullptr) {
            ui->downloadingListWidget->removeItemWidget(widgetItem);
        }
        w = ui->finishedListWidget->itemWidget(widgetItem);
        if (w != nullptr) {
            ui->finishedListWidget->removeItemWidget(widgetItem);
        }
    });
}
