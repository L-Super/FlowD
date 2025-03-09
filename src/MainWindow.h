#pragma once

#include <QList>
#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
    class MainWindow;
}
QT_END_NAMESPACE

class QButtonGroup;
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
    void newDownloadTask();

private:
    void minimizeSidebar(bool minimize);

private:
    Ui::MainWindow* ui;
    DownloadListWidget* downloadListWidget;
    SettingsWidget* settingsWidget;
    HelpWidget* helpWidget;
    QButtonGroup* buttonGroup;
    bool sidebarMinimized;// sidebar status
};
