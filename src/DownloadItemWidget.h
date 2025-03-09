#pragma once

#include "logic/DownloadItemInfo.h"
#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
    class DownloadItemWidget;
}
QT_END_NAMESPACE

class QListWidgetItem;

class DownloadItemWidget : public QWidget {
    Q_OBJECT

public:
    explicit DownloadItemWidget(size_t id, QWidget* parent = nullptr);
    ~DownloadItemWidget() override;

    void setDownloadTaskID(size_t id);
    size_t downloadTaskID() const;

    void setPauseButtonVisible(bool visible);

    void setDownloadItemInfo(const DownloadItemInfo& info);
    const DownloadItemInfo& downloadItemInfo() const;

signals:
    void removeItemSignal();
    void completeDownloadSignal();
    void progressUpdateFromTaskSignal(unsigned long total, unsigned long downloaded, unsigned long speed,
                                      double remainTime);
    void completeDownloadFromTaskSignal();

protected slots:
    void onPauseButtonClicked(bool checked);
    void onDeleteButtonClicked();
    void onOpenFileButtonClicked();
    void onMoreInfoButtonClicked();
    void onProgressUpdate(unsigned long total, unsigned long downloaded, unsigned long speed, double remainTime);
    void onCompleteDownload();

private:
    Ui::DownloadItemWidget* ui;
    size_t taskID;
    DownloadItemInfo fileInfo;
    const int progressMaximum;
};
