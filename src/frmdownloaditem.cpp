#include "frmdownloaditem.h"
#include "ui_frmdownloaditem.h"

FrmDownloadItem::FrmDownloadItem(QWidget* parent) : QWidget(parent), ui(new Ui::FrmDownloadItem)
{
    ui->setupUi(this);
}

FrmDownloadItem::~FrmDownloadItem()
{
    delete ui;
}
