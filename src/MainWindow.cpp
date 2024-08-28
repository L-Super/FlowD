#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QDebug>
#include <QDir>
#include <QMessageBox>
#include <QSettings>
#include <QStandardPaths>

#include "DownloadListWidget.h"
#include "HelpWidget.h"
#include "NewDownloadDialog.h"
#include "SettingsWidget.h"

MainWindow::MainWindow(QWidget* parent) : QWidget(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connectSlots();

    // Do not swap with the previous line of code, or it will result in Empty pointers.
    iniUi();
}

void MainWindow::connectSlots()
{
    // Add the pointer of the sidebar tab button to checkableToolButtons for convenient subsequent control
    checkableToolButtons.append(ui->toolDownloadList);
    checkableToolButtons.append(ui->toolSettings);
    checkableToolButtons.append(ui->toolHelp);

    for (int i = 0; i < checkableToolButtons.count(); ++i)
        connect(checkableToolButtons[i], &QToolButton::clicked, this, &MainWindow::onCheckableToolButtonsClicked);

    // Connect signals and slots of other widgets
    connect(ui->toolMenu, &QToolButton::clicked, this, &MainWindow::onToolMenuClicked);
    connect(ui->toolNewDownload, &QToolButton::clicked, this, [] {
        auto newDownloadDialog = new NewDownloadDialog;
        newDownloadDialog->exec();
        //I have already used setAttribute(Qt::WA_DeleteOnClose);
    });
}

void MainWindow::iniUi()
{
    // Set the display state of the tab
    QString cachePath = QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
    QDir cacheDir(cachePath);
    if (!cacheDir.exists())
        cacheDir.mkpath(cachePath);
    QSettings set(cachePath + "/config.ini", QSettings::IniFormat);
    tabMinimized = set.value("Common/MinimizeMainTab", false).toBool();
    minimizeMainTab(tabMinimized);

    // Add  QWidget to the StackedWidget
    downloadListWidget = new DownloadListWidget;
    settingsWidget = new SettingsWidget;
    helpWidget = new HelpWidget;

    ui->stackedWidget->insertWidget(0, downloadListWidget);
    ui->stackedWidget->insertWidget(1, settingsWidget);
    ui->stackedWidget->insertWidget(2, helpWidget);

    // Initially select "Download List"
    ui->stackedWidget->setCurrentIndex(0);
    checkableToolButtons[0]->setChecked(1);
}

// This function sets the display mode of the main tab to show only icons or both icons and text
void MainWindow::minimizeMainTab(bool minimize)
{
    if (minimize) {
        for (int i = 0; i < checkableToolButtons.length(); ++i) {
            checkableToolButtons[i]->setToolButtonStyle(Qt::ToolButtonIconOnly);
            checkableToolButtons[i]->setFixedWidth(40);
        }
        ui->toolNewDownload->setToolButtonStyle(Qt::ToolButtonIconOnly);
        ui->toolNewDownload->setFixedWidth(40);
    }
    else {
        for (int i = 0; i < checkableToolButtons.length(); ++i) {
            checkableToolButtons[i]->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
            checkableToolButtons[i]->setFixedWidth(200);
        }
        ui->toolNewDownload->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        ui->toolNewDownload->setFixedWidth(200);
    }

    // Save configuration to INI file
    QString cachePath = QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
    QDir cacheDir(cachePath);
    if (!cacheDir.exists())
        cacheDir.mkpath(cachePath);
    QSettings set(cachePath + "/config.ini", QSettings::IniFormat);
    set.setValue("Common/MinimizeMainTab", minimize);
}

void MainWindow::onCheckableToolButtonsClicked(bool checked)
{
    QToolButton* clickedToolButton = qobject_cast<QToolButton*>(sender());

    //Find pressed button
    for (int i = 0; i < checkableToolButtons.count(); ++i) {
        if (clickedToolButton == checkableToolButtons[i]) {
            // If the tab is clicked multiple times, maintain the selected property.
            if (!checked) {
                clickedToolButton->setChecked(true);
            }
            ui->stackedWidget->setCurrentIndex(i);
        }
        else {
            // Deselect tabs that are not selected
            checkableToolButtons[i]->setChecked(false);
        }
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
