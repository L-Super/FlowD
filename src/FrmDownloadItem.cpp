#include "FrmDownloadItem.h"
#include "ui_FrmDownloadItem.h"

FrmDownloadItem::FrmDownloadItem(QWidget* parent) : QWidget(parent), ui(new Ui::FrmDownloadItem)
{
    ui->setupUi(this);
    connectSlots();
}

void FrmDownloadItem::connectSlots()
{
    connect(ui->btnSuspend,&QPushButton::clicked,this,&FrmDownloadItem::onBtnSuspendClicked);
}

void FrmDownloadItem::onBtnSuspendClicked(bool checked)
{
    if(checked){
        ui->btnSuspend->setText("继续");
        //TODO:暂停下载的逻辑
    }else{
        ui->btnSuspend->setText("暂停");
        //TODO:开始下载的逻辑
    }
}

FrmDownloadItem::~FrmDownloadItem()
{
    delete ui;
}
