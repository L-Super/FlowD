#pragma once

#include <QList>
#include <QToolButton>
#include <QWidget>



QT_BEGIN_NAMESPACE
namespace Ui {
    class MainWindow;
}
QT_END_NAMESPACE

class DownloadListWidget;
class HelpWidget;
class SettingsWidget;

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
    DownloadListWidget* downloadListWidget;
    SettingsWidget* settingsWidget;
    HelpWidget* helpWidget;
    QList<QToolButton*> checkableToolButtons;//所有选项卡按钮
    // QTimer* timer;

    bool tabMinimized;//主选项卡状态

    void connectSlots();
    void iniUi();

    inline void minimizeMainTab(bool minimize);

    void setIconTheme(QPalette palette = QPalette());
    void setBlackIcon();
    void setWhiteIcon();

    void onSystemStyleChanged(QPalette palette);
};
