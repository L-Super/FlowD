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

void DownloadItemWidget::setFileName(QString fileName)
{
    this->fileName = fileName;
}
void DownloadItemWidget::setFileUrl(QString fileUrl)
{
    this->fileUrl = fileUrl;
}
void DownloadItemWidget::setSavedFilePath(QString filePath)
{
    this->savedFilePath = filePath;
}
void DownloadItemWidget::setFileDownloadProgress(qint64 totalBytes, qint64 downloadedBytes)
{
    this->qint64_totalBytes = totalBytes;
    this->qint64_downloadedBytes = downloadedBytes;
}
void DownloadItemWidget::setDownloadState(bool isDownloading)
{
    this->isDownloading = isDownloading;
}
void DownloadItemWidget::setRemainTime(qint64 remainTime)
{
    this->qint64_remainTime = remainTime;
}
void DownloadItemWidget::setDownloadSpeed(double downloadSpeed)
{
    this->double_downloadSpeed = downloadSpeed;
}

QString DownloadItemWidget::getFileName()
{
    return fileName;
}
QString DownloadItemWidget::getSavedFilePath()
{
    return savedFilePath;
}
QString DownloadItemWidget::getFileUrl()
{
    return fileUrl;
}
void DownloadItemWidget::getFileDownloadProgress(qint64& totalBytes, qint64& downloadedBytes)
{
    totalBytes = this->qint64_totalBytes;
    downloadedBytes = this->qint64_downloadedBytes;
}
bool DownloadItemWidget::getDownloadState()
{
    return isDownloading;
}
qint64 DownloadItemWidget::getRemainTime()
{
    return qint64_remainTime;
}
double DownloadItemWidget::getDownloadSpeed()
{
    return double_downloadSpeed;
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
