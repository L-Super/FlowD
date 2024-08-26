#include "DownloadItemWidget.h"
#include "ui_DownloadItemWidget.h"

DownloadItemWidget::DownloadItemWidget(QWidget* parent) : QWidget(parent), ui(new Ui::DownloadItemWidget)
{
    ui->setupUi(this);
    connectSlots();
}

void DownloadItemWidget::connectSlots()
{
    connect(ui->btnSuspend, &QPushButton::clicked, this, &DownloadItemWidget::onBtnSuspendClicked);
}

void DownloadItemWidget::onBtnSuspendClicked(bool checked)
{
    if (checked) {
        ui->btnSuspend->setText("继续");
        //TODO:暂停下载的逻辑
    }
    else {
        ui->btnSuspend->setText("暂停");
        //TODO:开始下载的逻辑
    }
}

DownloadItemWidget::~DownloadItemWidget()
{
    delete ui;
}
