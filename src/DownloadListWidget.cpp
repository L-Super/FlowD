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
    // 创建并添加到下载中列表
    QListWidgetItem* widgetItem = new QListWidgetItem();
    widgetItem->setSizeHint(item->sizeHint());
    ui->downloadingListWidget->addItem(widgetItem);
    ui->downloadingListWidget->setItemWidget(widgetItem, item);

    // 完成下载时的处理
    connect(item, &DownloadItemWidget::completeDownloadSignal, this, [this, widgetItem, item]() {
        // 获取当前下载项的必要信息
        auto downloadInfo = item->downloadItemInfo();
        auto id = item->downloadTaskID();
        
        // 从下载中列表移除并删除原项目
        int row = ui->downloadingListWidget->row(widgetItem);
        if (row >= 0) {
            QListWidgetItem* takenItem = ui->downloadingListWidget->takeItem(row);
            if (takenItem) {
                // 注意：这里会同时删除QListWidgetItem和其关联的DownloadItemWidget
                // 因为Qt的parent-child机制会自动删除子对象
                delete takenItem;
            }
        }

        // 创建新的下载完成项
        DownloadItemWidget* completedWidget = new DownloadItemWidget(id, ui->completedListWidget);
        completedWidget->setDownloadItemInfo(downloadInfo);
        completedWidget->setPauseButtonVisible(false);
        
        // 添加到已完成列表
        QListWidgetItem* newItem = new QListWidgetItem();
        newItem->setSizeHint(completedWidget->sizeHint());
        ui->completedListWidget->addItem(newItem);
        ui->completedListWidget->setItemWidget(newItem, completedWidget);
        
        // 连接新widget的删除信号
        connect(completedWidget, &DownloadItemWidget::removeItemSignal, this, [this, completedWidget]() {
            removeCompletedItem(completedWidget);
        });
    });

    // 移除下载中项目的处理
    connect(item, &DownloadItemWidget::removeItemSignal, this, [this, item]() {
        removeDownloadingItem(item);
    });
}

// 移除下载中项目
void DownloadListWidget::removeDownloadingItem(DownloadItemWidget* widget)
{
    for (int i = 0; i < ui->downloadingListWidget->count(); ++i) {
        QListWidgetItem* listItem = ui->downloadingListWidget->item(i);
        DownloadItemWidget* currentWidget = 
            qobject_cast<DownloadItemWidget*>(ui->downloadingListWidget->itemWidget(listItem));
            
        if (currentWidget == widget) {
            QListWidgetItem* takenItem = ui->downloadingListWidget->takeItem(i);
            if (takenItem) {
                delete takenItem;
            }
            break;
        }
    }
}

// 移除已完成项目
void DownloadListWidget::removeCompletedItem(DownloadItemWidget* widget)
{
    for (int i = 0; i < ui->completedListWidget->count(); ++i) {
        QListWidgetItem* listItem = ui->completedListWidget->item(i);
        DownloadItemWidget* currentWidget = 
            qobject_cast<DownloadItemWidget*>(ui->completedListWidget->itemWidget(listItem));
            
        if (currentWidget == widget) {
            QListWidgetItem* takenItem = ui->completedListWidget->takeItem(i);
            if (takenItem) {
                delete takenItem;
            }
            break;
        }
    }
}