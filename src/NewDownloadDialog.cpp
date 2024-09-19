#include "NewDownloadDialog.h"
#include "ui_NewDownloadDialog.h"

#include <QDebug>
#include <QStandardPaths>

#include "Logger.hpp"

NewDownloadDialog::NewDownloadDialog(QWidget* parent) : QDialog(parent), ui(new Ui::NewDownloadDialog)
{
    ui->setupUi(this);
    ui->savePathLineEdit->setPlaceholderText(QStandardPaths::writableLocation(QStandardPaths::DownloadLocation));
    showAdvanceOption(false);
    connect(ui->advancedCheckBox, &QCheckBox::clicked, this, &NewDownloadDialog::showAdvanceOption);
    connect(ui->okButton, &QPushButton::clicked, this, [this] {
        spdlog::info("accept");
        accept();
    });
    connect(ui->cancelButton, &QPushButton::clicked, this, [this] {
        spdlog::info("reject");
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
    }
}

NewDownloadDialog::~NewDownloadDialog()
{
    qDebug() << __func__;
    delete ui;
}

QString NewDownloadDialog::downloadUrl()
{
    if (ui->urlTextEdit->toPlainText().isEmpty())
        return {};
    //TODO:return clean url and check valid
    return ui->urlTextEdit->toPlainText();
}
