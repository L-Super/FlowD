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

    DownloadItemWidget* addDownloadItem(QString URL, QString fileName, QString fileSavedPath, qint64 totalBytes,
                                        qint64 downloadedBytes, bool isDownloading);
protected slots:
    void onRemoveFromWidgetRequested(QListWidgetItem* itemToRemove);

private:
    Ui::DownloadListWidget* ui;

    void iniUi();
    void connectSlots();
    void changeTab(int index);
};
