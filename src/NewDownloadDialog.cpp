#include "NewDownloadDialog.h"
#include "ui_NewDownloadDialog.h"

NewDownloadDialog::NewDownloadDialog(QWidget* parent) : QDialog(parent), ui(new Ui::NewDownloadDialog)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    resizeWidget(false);
    connectSlots();
}

void NewDownloadDialog::connectSlots()
{
    connect(ui->checkAdvanced,&QCheckBox::clicked,this,&NewDownloadDialog::resizeWidget);
}

void NewDownloadDialog::resizeWidget(bool maxMode)
{
    if(maxMode){
        ui->advancedWidget->show();
        setFixedHeight(623);
    }else{
        ui->advancedWidget->hide();
        setFixedHeight(310);
    }
}

NewDownloadDialog::~NewDownloadDialog()
{
    delete ui;
}


