#pragma once

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
    class DownloadListWidget;
}
QT_END_NAMESPACE

class DownloadItemWidget;

class DownloadListWidget : public QWidget {
    Q_OBJECT

public:
    explicit DownloadListWidget(QWidget* parent = nullptr);
    ~DownloadListWidget() override;

    void addDownloadingItem(DownloadItemWidget* item);

protected:
    void removeDownloadingItem(DownloadItemWidget* widget);
    void removeCompletedItem(DownloadItemWidget* widget);

private:
    Ui::DownloadListWidget* ui;
};
