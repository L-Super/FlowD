#include "frmdownloadlist.h"
#include "ui_frmdownloadlist.h"

FrmDownloadList::FrmDownloadList(QWidget* parent) : QWidget(parent), ui(new Ui::FrmDownloadList)
{
    ui->setupUi(this);
}

FrmDownloadList::~FrmDownloadList()
{
    delete ui;
}
