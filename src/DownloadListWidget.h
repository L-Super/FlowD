#pragma once

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
    class DownloadListWidget;
}
QT_END_NAMESPACE

class DownloadListWidget : public QWidget {
    Q_OBJECT

public:
    explicit DownloadListWidget(QWidget* parent = nullptr);
    ~DownloadListWidget() override;

private:
    Ui::DownloadListWidget* ui;

    void iniUi();
    void connectSlots();
    void changeTab(int index);
};
