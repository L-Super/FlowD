#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QButtonGroup>
#include <QDir>
#include <QMessageBox>
#include <QSettings>
#include <QTimer>

#include "DownloadListWidget.h"
#include "HelpWidget.h"
#include "NewDownloadDialog.h"
#include "SettingsWidget.h"

#include "Logger.hpp"
#include "Utils/Path.h"

MainWindow::MainWindow(QWidget* parent) : QWidget(parent), ui(new Ui::MainWindow), buttonGroup(new QButtonGroup(this))
{
    ui->setupUi(this);

    // Set the display state of the tab
    QSettings set(utils::Path::cachePath() + "/config.ini", QSettings::IniFormat);
    tabMinimized = set.value("Common/MinimizeMainTab", false).toBool();
    minimizeMainTab(tabMinimized);

    // Add QWidget to the StackedWidget
    downloadListWidget = new DownloadListWidget(this);
    settingsWidget = new SettingsWidget(this);
    helpWidget = new HelpWidget(this);

    ui->stackedWidget->insertWidget(0, downloadListWidget);
    ui->stackedWidget->insertWidget(1, settingsWidget);
    ui->stackedWidget->insertWidget(2, helpWidget);

    // Initially select "Download List"
    ui->stackedWidget->setCurrentIndex(0);

    buttonGroup->setExclusive(true);
    buttonGroup->addButton(ui->downloadListToolButton, 0);
    buttonGroup->addButton(ui->settingsToolButton, 1);
    buttonGroup->addButton(ui->helpToolButton, 2);
    connect(buttonGroup, &QButtonGroup::idClicked, this, &MainWindow::toolButtonClicked);

    connect(ui->menuToolButton, &QToolButton::clicked, this, &MainWindow::onToolMenuClicked);
    connect(ui->newDownloadToolButton, &QToolButton::clicked, this, [] {
        auto newDownloadDialog = new NewDownloadDialog;
        newDownloadDialog->exec();
        newDownloadDialog->deleteLater();
    });
}

// This function sets the display mode of the main tab to show only icons or both icons and text
void MainWindow::minimizeMainTab(bool minimize)
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
    tabMinimized = !tabMinimized;//Change State
    minimizeMainTab(tabMinimized);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::toolButtonClicked(int index)
{
    ui->stackedWidget->setCurrentIndex(index);
}
