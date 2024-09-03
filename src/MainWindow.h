#pragma once

#include <QList>
#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
    class MainWindow;
}
QT_END_NAMESPACE

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
    void onCheckableToolButtonsClicked(bool checked);
    void onToolMenuClicked();

private:
    void minimizeMainTab(bool minimize);
    void setIconTheme(QPalette palette = QPalette());
    void setBlackIcon();
    void setWhiteIcon();
    void onSystemStyleChanged(QPalette palette);

private:
    Ui::MainWindow* ui;
    DownloadListWidget* downloadListWidget;
    SettingsWidget* settingsWidget;
    HelpWidget* helpWidget;
    QList<QToolButton*> checkableToolButtons;//所有选项卡按钮

    bool tabMinimized;//主选项卡状态
};
