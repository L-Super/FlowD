#include "SettingsBasicWidget.h"
#include "ui_SettingsBasicWidget.h"

#include <QFileDialog>
#include <QSettings>
#include <QStandardPaths>
SettingsBasicWidget::SettingsBasicWidget(QWidget* parent) : QWidget(parent), ui(new Ui::SettingsBasicWidget)
{
    ui->setupUi(this);
    iniSettings();
    connectSlots();
}

SettingsBasicWidget::~SettingsBasicWidget()
{
    delete ui;
}

void SettingsBasicWidget::addToChangedHash(QString settingsPath, QVariant value)
{
    changedSettings.insert(settingsPath,value);
}

void SettingsBasicWidget::iniSettings()
{
    //Get cache path
    QString cachePath = QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
    QDir cacheDir(cachePath);
    if (!cacheDir.exists())
        cacheDir.mkpath(cachePath);
    QSettings set(cachePath + "/config.ini", QSettings::IniFormat);

    //Get display mode config
    int styleIndex = set.value("Basic/DisplayStyle", 0).toInt();
    ui->comboStyle->setCurrentIndex(styleIndex);

    //Get display language
    int language = set.value("Basic/Language", 0).toInt();
    ui->comboLanguage->setCurrentIndex(language);

    //Get file saving path
    QString savePath =
            set.value("Basic/SavePath", QStandardPaths::writableLocation(QStandardPaths::DownloadLocation)).toString();
    ui->lineSavePath->setText(savePath);

    //Get whether auto run feature is enabled
    bool autorun = set.value("Basic/EnableAutoRun", true).toBool();
    ui->checkAutoRun->setChecked(autorun);

    //Get whether unfinshed task continues to download on app starting
    bool continueDownload = set.value("Basic/AutoRunContinue", true).toBool();
    ui->checkAutoRunContinue->setChecked(continueDownload);

    //Get whether app automatically started display its UI
    bool autoDisplay = set.value("Basic/AutoRunDisplay", false).toBool();
    ui->checkAutoRunDisplay->setChecked(autoDisplay);

    //Get whether deleting file needs confirmation
    bool noNeedOk = set.value("Basic/AutoRunDeleteNoOK", false).toBool();
    ui->checkDeleteNoOk->setChecked(noNeedOk);

    //Get whether notification is needed when a task finishes
    bool notifyOnFinished = set.value("Basic/NotifyOnFinished", true).toBool();
    ui->checkNotifyOnFinished->setChecked(notifyOnFinished);

    //Get whether upgrade should automatically perform
    bool autoUpdate = set.value("Basic/AutoUpdate", true).toBool();
    ui->checkAutoUp->setChecked(autoUpdate);

    //Get whether jumping to download list interface is needed on a new task created
    bool jump = set.value("Basic/JumpOnNewTask", true).toBool();
    ui->checkJumpOnNewTask->setChecked(jump);

    //Get the count of download threads
    int count = set.value("Basic/ThreadCount", 8).toInt();
    ui->spinThreadCount->setValue(count);
}

void SettingsBasicWidget::connectSlots()
{
    connect(ui->comboStyle, &QComboBox::currentIndexChanged, this, [=](int index) {
        //Get cache path
        QString cachePath = QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
        QDir cacheDir(cachePath);
        if (!cacheDir.exists())
            cacheDir.mkpath(cachePath);
        QSettings set(cachePath + "/config.ini", QSettings::IniFormat);
        set.setValue("Basic/DisplayStyle", index);
    });

    connect(ui->comboLanguage, &QComboBox::currentIndexChanged, this, [=](int index) {
        //Get cache path
        QString cachePath = QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
        QDir cacheDir(cachePath);
        if (!cacheDir.exists())
            cacheDir.mkpath(cachePath);
        QSettings set(cachePath + "/config.ini", QSettings::IniFormat);
        set.setValue("Basic/Language", index);
    });

    connect(ui->btnSelectPath, &QPushButton::clicked, this, [=] {
        //Get cache path
        QString cachePath = QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
        QDir cacheDir(cachePath);
        if (!cacheDir.exists())
            cacheDir.mkpath(cachePath);
        QSettings set(cachePath + "/config.ini", QSettings::IniFormat);

        QString ordinaryPath =
                set.value("Basic/SavePath", QStandardPaths::writableLocation(QStandardPaths::DownloadLocation))
                        .toString();
        QString selectedPath=QFileDialog::getExistingDirectory(this,tr("Select a folder for file saving"),
                                                         ordinaryPath);
        if(!selectedPath.isEmpty() && ordinaryPath!=selectedPath){
            ui->lineSavePath->setText(QDir::toNativeSeparators(selectedPath));
            set.setValue("Basic/SavePath",selectedPath);    //record selected path
        }
    });

    connect(ui->checkAutoRun, &QCheckBox::clicked, this, [=](bool checked) {
        //Get cache path
        QString cachePath = QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
        QDir cacheDir(cachePath);
        if (!cacheDir.exists())
            cacheDir.mkpath(cachePath);
        QSettings set(cachePath + "/config.ini", QSettings::IniFormat);

        set.setValue("Basic/EnableAutoRun", checked);
    });

    connect(ui->checkAutoRunContinue, &QCheckBox::clicked, this, [=](bool checked) {
        //Get cache path
        QString cachePath = QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
        QDir cacheDir(cachePath);
        if (!cacheDir.exists())
            cacheDir.mkpath(cachePath);
        QSettings set(cachePath + "/config.ini", QSettings::IniFormat);

        set.setValue("Basic/AutoRunContinue", checked);
    });

    connect(ui->checkAutoRunDisplay, &QCheckBox::clicked, this, [=](bool checked) {
        //Get cache path
        QString cachePath = QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
        QDir cacheDir(cachePath);
        if (!cacheDir.exists())
            cacheDir.mkpath(cachePath);
        QSettings set(cachePath + "/config.ini", QSettings::IniFormat);

        set.setValue("Basic/AutoRunDisplay", checked);
    });

    connect(ui->checkDeleteNoOk, &QCheckBox::clicked, this, [=](bool checked) {
        //Get cache path
        QString cachePath = QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
        QDir cacheDir(cachePath);
        if (!cacheDir.exists())
            cacheDir.mkpath(cachePath);
        QSettings set(cachePath + "/config.ini", QSettings::IniFormat);

        set.setValue("Basic/AutoRunDeleteNoOK", checked);
    });

    connect(ui->checkJumpOnNewTask, &QCheckBox::clicked, this, [=](bool checked) {
        //Get cache path
        QString cachePath = QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
        QDir cacheDir(cachePath);
        if (!cacheDir.exists())
            cacheDir.mkpath(cachePath);
        QSettings set(cachePath + "/config.ini", QSettings::IniFormat);

        set.setValue("Basic/JumpOnNewTask", checked);
    });

    connect(ui->checkNotifyOnFinished, &QCheckBox::clicked, this, [=](bool checked) {
        //Get cache path
        QString cachePath = QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
        QDir cacheDir(cachePath);
        if (!cacheDir.exists())
            cacheDir.mkpath(cachePath);
        QSettings set(cachePath + "/config.ini", QSettings::IniFormat);

        set.setValue("Basic/NotifyOnFinished", checked);
    });

    connect(ui->checkAutoUp, &QCheckBox::clicked, this, [=](bool checked) {
        //Get cache path
        QString cachePath = QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
        QDir cacheDir(cachePath);
        if (!cacheDir.exists())
            cacheDir.mkpath(cachePath);
        QSettings set(cachePath + "/config.ini", QSettings::IniFormat);

        set.setValue("Basic/AutoUpdate", checked);
    });

    connect(ui->lineSavePath, &QLineEdit::textChanged, this, [=](QString text) {
        //Get cache path
        QString cachePath = QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
        QDir cacheDir(cachePath);
        if (!cacheDir.exists())
            cacheDir.mkpath(cachePath);
        QSettings set(cachePath + "/config.ini", QSettings::IniFormat);

        set.setValue("Basic/SavePath", text);
    });

    connect(ui->spinThreadCount, &QSpinBox::valueChanged, [=](int count) {
        //Get cache path
        QString cachePath = QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
        QDir cacheDir(cachePath);
        if (!cacheDir.exists())
            cacheDir.mkpath(cachePath);
        QSettings set(cachePath + "/config.ini", QSettings::IniFormat);

        set.setValue("Basic/ThreadCount", count);
    });
}
