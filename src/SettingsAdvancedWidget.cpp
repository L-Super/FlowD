#include "SettingsAdvancedWidget.h"
#include "ui_SettingsAdvancedWidget.h"

#include <QDir>
#include <QSettings>
#include <QStandardPaths>

#include "UserAgentConfigDialog.h"

SettingsAdvancedWidget::SettingsAdvancedWidget(QWidget* parent) : QWidget(parent), ui(new Ui::SettingsAdvancedWidget)
{
    ui->setupUi(this);
    connectSlots();
}

void SettingsAdvancedWidget::connectSlots()
{
    connect(ui->btnUserAgent, &QPushButton::clicked, this, [] {
        UserAgentConfigDialog userconfigDialog;
        QString userAgent = userconfigDialog.getUserAgent();
        //Get cache path
        QString cachePath = QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
        QDir cacheDir(cachePath);
        if (!cacheDir.exists())
            cacheDir.mkpath(cachePath);
        QSettings set(cachePath + "/config.ini", QSettings::IniFormat);

        set.setValue("Advanced/user-agent", userAgent);
    });
}

SettingsAdvancedWidget::~SettingsAdvancedWidget()
{
    delete ui;
}
