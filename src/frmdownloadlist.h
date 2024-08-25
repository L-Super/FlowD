#pragma once

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
    class FrmDownloadList;
}
QT_END_NAMESPACE

class FrmDownloadList : public QWidget {
    Q_OBJECT

public:
    explicit FrmDownloadList(QWidget* parent = nullptr);
    ~FrmDownloadList() override;

protected slots:
    // void onBtnTabDownloadingClicked(bool checked);
    // void onBtnTabStoppedClicked(bool checked);
private:
    Ui::FrmDownloadList* ui;

    void iniUi();
    void connectSlots();
    void changeTab(int index);
};
