#pragma once

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
    class DownloadItemWidget;
}
QT_END_NAMESPACE

class DownloadItemWidget : public QWidget {
    Q_OBJECT

public:
    explicit DownloadItemWidget(QWidget* parent = nullptr);
    ~DownloadItemWidget() override;

protected slots:
    void onBtnSuspendClicked(bool checked);

private:
    Ui::DownloadItemWidget* ui;

    void connectSlots();
};
