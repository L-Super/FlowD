#include "NewDownloadDialog.h"
#include "ui_NewDownloadDialog.h"

#include <QDebug>

#include "Logger.hpp"

NewDownloadDialog::NewDownloadDialog(QWidget* parent) : QDialog(parent), ui(new Ui::NewDownloadDialog)
{
    ui->setupUi(this);
    showAdvanceOption(false);
    connect(ui->advancedCheckBox, &QCheckBox::clicked, this, &NewDownloadDialog::showAdvanceOption);
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
    //TODO:return clean url and check valid
    return ui->urlTextEdit->placeholderText();
}
