#pragma once

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
    class DownloadListWidget;
}
QT_END_NAMESPACE

class QListWidgetItem;

class DownloadItemWidget;

class DownloadListWidget : public QWidget {
    Q_OBJECT

public:
    explicit DownloadListWidget(QWidget* parent = nullptr);
    ~DownloadListWidget() override;

    int addDownloadItem(QString URL, QString fileName, QString fileSavedPath, qint64 totalBytes, qint64 downloadedBytes,
                        bool isDownloading);

private:
    Ui::DownloadListWidget* ui;

    //一个存储了列表条目和自定义条目映射关系的QHash
    QHash<QListWidgetItem*, DownloadItemWidget*> itemsHash;
    QList<QListWidgetItem*> listItems;
    void iniUi();
    void connectSlots();
    void changeTab(int index);
};
