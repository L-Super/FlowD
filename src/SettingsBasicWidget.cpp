#include "SettingsBasicWidget.h"
#include "ui_SettingsBasicWidget.h"

#include <QFileDialog>
#include <QStandardPaths>

#include "AppConfig.h"
#include "LanguageCode.hpp"
#include "Logger.hpp"

SettingsBasicWidget::SettingsBasicWidget(QWidget* parent) : QWidget(parent), ui(new Ui::SettingsBasicWidget)
{
    ui->setupUi(this);

    ui->languageComboBox->addItem(tr("System Language"));
    for (const auto& lan : lan::languageCode) {
        ui->languageComboBox->addItem(QString::fromStdString(lan.second));
    }
    ui->savePathLineEdit->setPlaceholderText(QStandardPaths::writableLocation(QStandardPaths::DownloadLocation));

    initSettings();

#if QT_VERSION <= QT_VERSION_CHECK(6, 0, 0)
    connect(ui->styleComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this](int index) {
        // TODO: change theme
        spdlog::info("User change default theme to {}", index);
        AppConfig::instance().setBasic("style", index);
    });
#else
    connect(ui->styleComboBox, &QComboBox::currentIndexChanged, this, [this](int index) {
        // TODO: change theme
        spdlog::info("User change default theme to {}", index);
        AppConfig::instance().setBasic("style", index);
    });
#endif
    connect(ui->languageComboBox, &QComboBox::currentTextChanged, this, [this](const QString& index) {
        // TODO: change language
        spdlog::info("User change default language to {}", index.toStdString());
        AppConfig::instance().setBasic("language", index.toStdString());
    });
    connect(ui->selectPathButton, &QPushButton::clicked, this, [this] {
        QString defaultPath = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);
        QString selectedPath =
            QFileDialog::getExistingDirectory(this, tr("Select a folder for file saving"), defaultPath);
        if (!selectedPath.isEmpty() && defaultPath != selectedPath) {
            ui->savePathLineEdit->setText(QDir::toNativeSeparators(selectedPath));
            AppConfig::instance().setBasic("save_path", selectedPath.toStdString());
        }
    });

    // TODO: when leave this widget, check it weather be changed, if changed, save it
}

SettingsBasicWidget::~SettingsBasicWidget()
{
    delete ui;
}

void SettingsBasicWidget::initSettings()
{
    auto style = AppConfig::instance().getBasic<int>("style");
    if (style.has_value()) {
        ui->styleComboBox->setCurrentIndex(style.value());
    }
    auto language = AppConfig::instance().getBasic<std::string>("language");
    if (language.has_value()) {
        ui->languageComboBox->setCurrentText(QString::fromStdString(language.value()));
    }

    auto autoStartup = AppConfig::instance().getBasic<bool>("auto_startup");
    if (autoStartup.has_value()) {
        ui->autoStartupCheckBox->setChecked(autoStartup.value());
    }

    auto autoContinue = AppConfig::instance().getBasic<bool>("auto_continue");
    if (autoContinue.has_value()) {
        ui->autoStartupCheckBox->setChecked(autoContinue.value());
    }

    auto savePath = AppConfig::instance().getBasic<std::string>("save_path");
    if (savePath.has_value()) {
        ui->savePathLineEdit->setText(QString::fromStdString(savePath.value()));
    }

    auto notifyOnFinished = AppConfig::instance().getBasic<bool>("notify_on_finished");
    if (notifyOnFinished.has_value()) {
        ui->notifyOnFinishedCheckBox->setChecked(notifyOnFinished.value());
    }

    auto autoUpgrade = AppConfig::instance().getBasic<bool>("auto_upgrade");
    if (notifyOnFinished.has_value()) {
        ui->autoUpgradeCheckBox->setChecked(notifyOnFinished.value());
    }

    auto threadCount = AppConfig::instance().getBasic<int>("thread_count");
    if (threadCount.has_value()) {
        ui->threadCountSpinBox->setValue(threadCount.value());
    }
}
