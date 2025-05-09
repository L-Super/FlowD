#include "NewDownloadDialog.h"
#include "ui_NewDownloadDialog.h"

#include <QClipboard>
#include <QStandardPaths>
#include <thread>

#include "Logger.hpp"

NewDownloadDialog::NewDownloadDialog(QWidget* parent) : QDialog(parent), ui(new Ui::NewDownloadDialog)
{
    ui->setupUi(this);

    ui->savePathLineEdit->setPlaceholderText(QStandardPaths::writableLocation(QStandardPaths::DownloadLocation));
    unsigned int cpuCoreCount = std::thread::hardware_concurrency();
    ui->spinThreadCount->setMinimum(1);
    ui->spinThreadCount->setMaximum(static_cast<int>(cpuCoreCount) * 2);
    ui->spinThreadCount->setValue(static_cast<int>(cpuCoreCount));

    setFixedWidth(600);

    showAdvanceOption(false);

    if (auto text = QApplication::clipboard()->text(); !text.isEmpty()) {
        ui->urlTextEdit->setPlainText(text.trimmed());
    }

    connect(ui->advancedCheckBox, &QCheckBox::clicked, this, &NewDownloadDialog::showAdvanceOption);
    connect(ui->okButton, &QPushButton::clicked, this, [this] {
        accept();
    });
    connect(ui->cancelButton, &QPushButton::clicked, this, [this] {
        reject();
    });
}

void NewDownloadDialog::showAdvanceOption(bool maxMode)
{
    if (maxMode) {
        ui->advancedWidget->show();
    }
    else {
        ui->advancedWidget->hide();

        // resize
        adjustSize();
    }
}

NewDownloadDialog::~NewDownloadDialog()
{
    delete ui;
}

QString NewDownloadDialog::downloadUrl()
{
    if (ui->urlTextEdit->toPlainText().isEmpty())
        return {};
    // trimmed text
    auto url = ui->urlTextEdit->toPlainText().trimmed();
    return url;
}

QString NewDownloadDialog::saveFilePath()
{
    return ui->savePathLineEdit->text();
}

unsigned int NewDownloadDialog::threadCount()
{
    return ui->spinThreadCount->value();
}
