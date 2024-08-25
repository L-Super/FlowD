#pragma once

#include <QList>
#include <QToolButton>
#include <QWidget>

#include "FrmDownloadList.h"
#include "FrmHelp.h"
#include "FrmSettings.h"

QT_BEGIN_NAMESPACE
namespace Ui {
    class MainWindow;
}
QT_END_NAMESPACE


class MainWindow : public QWidget {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override;

protected slots:
    void onCheckableToolButtonsClicked(bool checked);
    void onToolMenuClicked();

private:
    Ui::MainWindow* ui;
    FrmDownloadList* frmDownloadList;
    FrmSettings* frmSettings;
    FrmHelp* frmHelp;
    QList<QToolButton*> checkableToolButtons;//所有选项卡按钮

    bool tabMinimized;//主选项卡状态

    void connectSlots();
    void iniUi();

    inline void minimizeMainTab(bool minimize);
};
