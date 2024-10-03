#include "DownloadListWidget.h"
#include "ui_DownloadListWidget.h"

#include <QListWidgetItem>
#include <QTabBar>

#include "DownloadItemWidget.h"

DownloadListWidget::DownloadListWidget(QWidget* parent) : QWidget(parent), ui(new Ui::DownloadListWidget)
{
    ui->setupUi(this);

    ui->tabWidget->setCurrentIndex(0);
    // expand tab bar
    ui->tabWidget->setDocumentMode(true);
    ui->tabWidget->tabBar()->setExpanding(true);
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

    connect(item, &DownloadItemWidget::completeDownloadSignal, this, [this, widgetItem]() {
        DownloadItemWidget* downloadWidget =
                qobject_cast<DownloadItemWidget*>(ui->downloadingListWidget->itemWidget(widgetItem));
        if (!downloadWidget) {
            return;
        }

        QListWidgetItem* newItem = new QListWidgetItem(ui->completedListWidget);
        ui->completedListWidget->addItem(newItem);

        ui->completedListWidget->setItemWidget(newItem, downloadWidget);
    });
    connect(item, &DownloadItemWidget::removeItemSignal, this, [this, widgetItem]() {
        auto w = ui->downloadingListWidget->itemWidget(widgetItem);
        if (w != nullptr) {
            ui->downloadingListWidget->removeItemWidget(widgetItem);
        }
        w = ui->completedListWidget->itemWidget(widgetItem);
        if (w != nullptr) {
            ui->completedListWidget->removeItemWidget(widgetItem);
        }
    });
}
