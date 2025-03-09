#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QButtonGroup>
#include <QDir>
#include <QStandardPaths>
#include <QTimer>

#include "AppConfig.h"
#include "DownloadItemWidget.h"
#include "DownloadListWidget.h"
#include "HelpWidget.h"
#include "NewDownloadDialog.h"
#include "SettingsWidget.h"
#include "logic/DownloadManager.h"
#include "utils/Logger.hpp"
#include "utils/Path.h"

MainWindow::MainWindow(QWidget* parent)
    : QWidget(parent), ui(new Ui::MainWindow), buttonGroup(new QButtonGroup(this)), sidebarMinimized(false)
{
    ui->setupUi(this);

    downloadListWidget = new DownloadListWidget(this);
    settingsWidget = new SettingsWidget(this);
    helpWidget = new HelpWidget(this);

    // Add QWidget to the StackedWidget
    ui->stackedWidget->insertWidget(0, downloadListWidget);
    ui->stackedWidget->insertWidget(1, settingsWidget);
    ui->stackedWidget->insertWidget(2, helpWidget);

    ui->stackedWidget->setCurrentIndex(0);

    buttonGroup->setExclusive(true);
    buttonGroup->addButton(ui->downloadListToolButton, 0);
    buttonGroup->addButton(ui->settingsToolButton, 1);
    buttonGroup->addButton(ui->helpToolButton, 2);

    ui->downloadListToolButton->setChecked(true);

    auto isMinimizeSidebar = AppConfig::instance().getBasic<bool>("minimizeSidebar");
    if (isMinimizeSidebar.has_value()) {
        sidebarMinimized = isMinimizeSidebar.value();
    }
    minimizeSidebar(sidebarMinimized);

    connect(buttonGroup, &QButtonGroup::idClicked, this, &MainWindow::toolButtonClicked);
    connect(ui->menuToolButton, &QToolButton::clicked, this, &MainWindow::onToolMenuClicked);
    connect(ui->newDownloadToolButton, &QToolButton::clicked, this, &MainWindow::newDownloadTask);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// This function sets the display mode of the main tab to show only icons or both icons and text
void MainWindow::minimizeSidebar(bool minimize)
{
    if (minimize) {
        for (const auto& button: buttonGroup->buttons()) {
            qobject_cast<QToolButton*>(button)->setToolButtonStyle(Qt::ToolButtonIconOnly);
            qobject_cast<QToolButton*>(button)->setFixedWidth(40);
        }
        ui->newDownloadToolButton->setToolButtonStyle(Qt::ToolButtonIconOnly);
        ui->newDownloadToolButton->setFixedWidth(40);
    }
    else {
        for (const auto& button: buttonGroup->buttons()) {
            qobject_cast<QToolButton*>(button)->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
            qobject_cast<QToolButton*>(button)->setFixedWidth(200);
        }
        ui->newDownloadToolButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        ui->newDownloadToolButton->setFixedWidth(200);
    }
}

void MainWindow::onToolMenuClicked()
{
    sidebarMinimized = !sidebarMinimized;
    minimizeSidebar(sidebarMinimized);
    AppConfig::instance().setBasic<bool>("minimizeSidebar", sidebarMinimized);
}

void MainWindow::toolButtonClicked(int index)
{
    ui->stackedWidget->setCurrentIndex(index);
}

void MainWindow::newDownloadTask()
{
    QScopedPointer<NewDownloadDialog> newDownloadDialog(new NewDownloadDialog);

    if (newDownloadDialog->exec() == QDialog::Rejected) {
        return;
    }

    auto url = newDownloadDialog->downloadUrl().toStdString();
    if (url.empty()) {
        spdlog::warn("New download task, but url is empty");
        return;
    }
    spdlog::info("New download task, url:{}", url);

    QString saveFileDir = newDownloadDialog->saveFilePath();
    if (saveFileDir.isEmpty()) {
        saveFileDir = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);
    }
    auto threadCount = newDownloadDialog->threadCount();

    // add download task to DownloadManager
    DownloadItemInfo itemInfo;
    itemInfo.url = url;
    itemInfo.saveDir = saveFileDir.toStdString();
    itemInfo.threadNum = threadCount;

    auto id = DownloadManager::instance().addTask(itemInfo);
    DownloadItemWidget* item = new DownloadItemWidget(id, downloadListWidget);

    downloadListWidget->addDownloadingItem(item);

    QMetaObject::invokeMethod(item, [item, id] {
        if (auto taskInfo = DownloadManager::instance().downloadTaskInfo(id); taskInfo.has_value()) {
            const auto& fileInfo = taskInfo.value();
            item->setDownloadItemInfo(fileInfo);
        }
        else {
            spdlog::warn("Get null file info from DownloadManager");
        }
    });

    DownloadManager::instance().setDownloadCompleteCallback(id, [id, item]() {
        emit item->completeDownloadFromTaskSignal();
    });
    DownloadManager::instance().setProgressCallback(
            id, [item](auto total, auto downloaded, auto speed, auto remainingTime) {
                emit item->progressUpdateFromTaskSignal(total, downloaded, speed, remainingTime);
            });
    DownloadManager::instance().startTask(id);
}
