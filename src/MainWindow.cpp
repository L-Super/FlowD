#include "MainWindow.h"
#include "ui_MainWindow.h"

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

MainWindow::MainWindow(QWidget* parent) : QWidget(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Set the display state of the tab
    QSettings set(utils::Path::cachePath() + "/config.ini", QSettings::IniFormat);
    tabMinimized = set.value("Common/MinimizeMainTab", false).toBool();

    // Add QWidget to the StackedWidget
    downloadListWidget = new DownloadListWidget(this);
    settingsWidget = new SettingsWidget(this);
    helpWidget = new HelpWidget(this);

    ui->stackedWidget->insertWidget(0, downloadListWidget);
    ui->stackedWidget->insertWidget(1, settingsWidget);
    ui->stackedWidget->insertWidget(2, helpWidget);

    // Initially select "Download List"
    ui->stackedWidget->setCurrentIndex(0);

    setIconTheme();

    checkableToolButtons.append(ui->downloadListToolButton);
    checkableToolButtons.append(ui->settingsToolButton);
    checkableToolButtons.append(ui->helpToolButton);
    checkableToolButtons[0]->setChecked(true);
    // Add the pointer of the sidebar tab button to checkableToolButtons for convenient later control
    for (int i = 0; i < checkableToolButtons.count(); ++i)
        connect(checkableToolButtons[i], &QToolButton::clicked, this, &MainWindow::onCheckableToolButtonsClicked);

    minimizeMainTab(tabMinimized);

    //  Detect whether the system theme has changed
    connect(qApp, &QApplication::paletteChanged, this, &MainWindow::setIconTheme);
    connect(qApp, &QApplication::paletteChanged, this, &MainWindow::onSystemStyleChanged);
    connect(ui->menuToolButton, &QToolButton::clicked, this, &MainWindow::onToolMenuClicked);
    connect(ui->newDownloadToolButton, &QToolButton::clicked, this, [] {
        auto newDownloadDialog = new NewDownloadDialog;
        newDownloadDialog->exec();
    });
}

// This function sets the display mode of the main tab to show only icons or both icons and text
void MainWindow::minimizeMainTab(bool minimize)
{
    if (minimize) {
        for (int i = 0; i < checkableToolButtons.length(); ++i) {
            checkableToolButtons[i]->setToolButtonStyle(Qt::ToolButtonIconOnly);
            checkableToolButtons[i]->setFixedWidth(40);
        }
        ui->newDownloadToolButton->setToolButtonStyle(Qt::ToolButtonIconOnly);
        ui->newDownloadToolButton->setFixedWidth(40);
    }
    else {
        for (int i = 0; i < checkableToolButtons.length(); ++i) {
            checkableToolButtons[i]->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
            checkableToolButtons[i]->setFixedWidth(200);
        }
        ui->newDownloadToolButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        ui->newDownloadToolButton->setFixedWidth(200);
    }

    // Save configuration to INI file
    QSettings set(utils::Path::cachePath() + "/config.ini", QSettings::IniFormat);
    set.setValue("Common/MinimizeMainTab", minimize);
}

void MainWindow::setIconTheme(QPalette palette)
{
    QSettings set(utils::Path::cachePath() + "/config.ini", QSettings::IniFormat);
    int index = set.value("Basic/DisplayStyle", 0).toInt();
    QColor color = palette.color(QPalette::Window);
    switch (index) {
        case 0:                           //Follow System Theme
            if (color.lightness() < 128) {//Determine System Color Theme
                setWhiteIcon();
            }
            else {
                setBlackIcon();
            }
            break;
        case 1://Light Theme
            setBlackIcon();
            break;
        case 2://Dark Theme
            setWhiteIcon();
            break;
        default:
            set.setValue("Basic/DisplayStyle", 0);
            setIconTheme(palette);
            break;
    }
}

void MainWindow::setBlackIcon()
{
    ui->menuToolButton->setIcon(QIcon(":/MainWindow/Resources/icons/white/menu.svg"));
    ui->newDownloadToolButton->setIcon(QIcon(":/MainWindow/Resources/icons/white/new.svg"));
    ui->downloadListToolButton->setIcon(QIcon(":/MainWindow/Resources/icons/white/downloadlist.svg"));
    ui->settingsToolButton->setIcon(QIcon(":/MainWindow/Resources/icons/white/settings.svg"));
    ui->helpToolButton->setIcon(QIcon(":/MainWindow/Resources/icons/white/help.svg"));
}

void MainWindow::setWhiteIcon()
{
    ui->menuToolButton->setIcon(QIcon(":/MainWindow/Resources/icons/black/menu.svg"));
    ui->newDownloadToolButton->setIcon(QIcon(":/MainWindow/Resources/icons/black/new.svg"));
    ui->downloadListToolButton->setIcon(QIcon(":/MainWindow/Resources/icons/black/downloadlist.svg"));
    ui->settingsToolButton->setIcon(QIcon(":/MainWindow/Resources/icons/black/settings.svg"));
    ui->helpToolButton->setIcon(QIcon(":/MainWindow/Resources/icons/black/help.svg"));
}

void MainWindow::onSystemStyleChanged(QPalette palette)
{
    QSettings set(utils::Path::cachePath() + "/config.ini", QSettings::IniFormat);
    int index = set.value("Basic/DisplayStyle", 0).toInt();
    QFile qssFile;
    QColor color = palette.color(QPalette::Window);
    //Follow System Theme
    if (index == 0) {
        if (color.lightness() < 128) {//Determine System Color Theme
            qssFile.setFileName(":/qss/Resources/style/style_black.qss");
        }
        else {
            qssFile.setFileName(":/qss/Resources/style/style_white.qss");
        }
        QString styleSheet;
        if (qssFile.open(QIODevice::ReadOnly)) {
            styleSheet = QString::fromLatin1(qssFile.readAll());
            qApp->setStyleSheet(styleSheet);
            qssFile.close();
        }
        else {
            spdlog::error("Failed in getting stylesheet!");
            QMessageBox::critical(nullptr, QObject::tr("Error"), QObject::tr("Failed in getting style sheet!"));
        }
    }
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
