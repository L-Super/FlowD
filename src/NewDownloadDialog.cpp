#include "NewDownloadDialog.h"
#include "ui_NewDownloadDialog.h"

#include "Logger.hpp"

NewDownloadDialog::NewDownloadDialog(QWidget* parent) : QDialog(parent), ui(new Ui::NewDownloadDialog)
{
    ui->setupUi(this);
    resizeWidget(false);
    connect(ui->advancedCheckBox, &QCheckBox::clicked, this, &NewDownloadDialog::resizeWidget);
}

void NewDownloadDialog::resizeWidget(bool maxMode)
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
    spdlog::info(__func__);
    delete ui;
}
