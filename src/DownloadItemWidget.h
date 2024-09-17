#pragma once

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

    size_t downloadTaskID();

    void hidePauseButton(bool hide);

protected:
    void onCompleteDownload();

signals:
    void removeItemSignal();
    void completeDownloadSignal();

protected slots:
    void onPauseButtonClicked(bool checked);

    void onDeleteButtonClicked();

    void onOpenFileButtonClicked();

    void onMoreInfoButtonClicked();

private:
    Ui::DownloadItemWidget* ui;
    size_t taskID;
};
