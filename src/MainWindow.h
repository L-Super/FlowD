#pragma once

#include <QList>
#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
    class MainWindow;
}
QT_END_NAMESPACE

class QButtonGroup;
class QToolButton;
class DownloadListWidget;
class HelpWidget;
class SettingsWidget;

class MainWindow : public QWidget {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override;

protected slots:
    void onToolMenuClicked();
    void toolButtonClicked(int index);

private:
    void minimizeMainTab(bool minimize);

private:
    Ui::MainWindow* ui;
    DownloadListWidget* downloadListWidget;
    SettingsWidget* settingsWidget;
    HelpWidget* helpWidget;
    QList<QToolButton*> checkableToolButtons;//所有选项卡按钮

    bool tabMinimized;//主选项卡状态
    QButtonGroup * buttonGroup;
};
