#pragma once

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
    class FrmDownloadItem;
}
QT_END_NAMESPACE


class FrmDownloadItem : public QWidget {
    Q_OBJECT

public:
    explicit FrmDownloadItem(QWidget* parent = nullptr);
    ~FrmDownloadItem() override;

protected slots:
    void onBtnSuspendClicked(bool checked);

private:
    Ui::FrmDownloadItem* ui;

    void connectSlots();
};
