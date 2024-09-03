#include "DownloadItemWidget.h"
#include "ui_DownloadItemWidget.h"

#include <QDesktopServices>
#include <QDir>
#include <QFileInfo>
#include <QListWidgetItem>
#include <QMessageBox>
#include <QUrl>

#include "Logger.hpp"

//TODO:Calculate remaining downloading time and download speed(perhaps)
DownloadItemWidget::DownloadItemWidget(QString URL, QString fileName, QString fileSavedPath, qint64 totalBytes,
                                       qint64 downloadedBytes, bool isDownloading, QListWidgetItem* listItem,
                                       QWidget* parent)
    : QWidget(parent), ui(new Ui::DownloadItemWidget)
{
    this->fileUrl = URL;
    this->fileName = fileName;
    this->savedFilePath = fileSavedPath;
    this->isDownloading = isDownloading;
    this->qint64_totalBytes = totalBytes;
    this->qint64_downloadedBytes = downloadedBytes;
    this->listItem = listItem;

    ui->setupUi(this);
    iniUi();
    connectSlots();
}

void DownloadItemWidget::connectSlots()
{
    connect(ui->btnSuspend, &QPushButton::clicked, this, &DownloadItemWidget::onBtnSuspendClicked);
    connect(ui->btnDelete, &QPushButton::clicked, this, &DownloadItemWidget::onBtnDeleteClicked);
    connect(ui->btnO, &QPushButton::clicked, this, &DownloadItemWidget::onBtnOpenClicked);
    connect(ui->btnMore, &QPushButton::clicked, this, &DownloadItemWidget::onBtnMoreClicked);
}

void DownloadItemWidget::setFileName(QString fileName)
{
    this->fileName = fileName;
    ui->labFileName->setText(this->fileName);
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

    ui->progressBar->setMaximum(totalBytes);
    ui->progressBar->setMaximum(downloadedBytes);

    std::pair<double, QString> resultTotal = convertToReasonableUnit(totalBytes);
    //std::pair<double,QString> resultDownloaded=convertToReasonableUnit(downloadedBytes);

    ui->labFileSize->setText(tr("File Size: %1 %2").arg(resultTotal.first, 0, 'f', 2).arg(resultTotal.second));
    //TODO:Update UI of downloaded bytes
}
void DownloadItemWidget::setDownloadState(bool isDownloading)
{
    //Update State
    this->isDownloading = isDownloading;

    //Update UI
    if (isDownloading) {
        ui->btnSuspend->show();
        ui->btnO->hide();
    }
    else {
        ui->btnSuspend->hide();
        ui->btnO->show();
    }
}
void DownloadItemWidget::setRemainTime()
{

}
void DownloadItemWidget::setDownloadSpeed(double downloadSpeed)
{
    this->double_downloadSpeed = downloadSpeed;

    std::pair<double, QString> result = convertToReasonableUnit(this->double_downloadSpeed);
    ui->labSpeed->setText(tr("Speed: %1 %2/s").arg(result.first, 0, 'f', 2).arg(result.second));
}
//This function converts byte sizes to more readable units.
std::pair<double, QString> DownloadItemWidget::convertToReasonableUnit(double bytesToConvert)
{
    QString unit;
    if (bytesToConvert >= 1024 * 1024 * 1024) {
        bytesToConvert /= (1024.00 * 1024.00 * 1024.00);
        unit = "GB";
    }
    else if (bytesToConvert >= 1024 * 1024) {
        bytesToConvert /= (1024.00 * 1024.00);
        unit = "MB";
    }
    else if (bytesToConvert >= 1024) {
        bytesToConvert /= 1024.00;
        unit = "KB";
    }
    else {
        unit = "B";
    }
    return {bytesToConvert, unit};
}

QString DownloadItemWidget::convertToReasonableTimeUnit(qint64 sec,qint64 min,qint64 hour,qint64 day)
{
    min += sec / 60;
    sec = sec % 60;

    hour += min / 60;
    min = min % 60;

    day += hour / 24;
    hour = hour % 24;

    QString result;
    if (day != 0) {
        result = QObject::tr("%1 Day %2 Hours %3 Minutes %4 Seconds").arg(day).arg(hour).arg(min).arg(sec);
    } else if (hour != 0) {
        result = QObject::tr("%1 Hours %2 Minutes %3 Seconds").arg(hour).arg(min).arg(sec);
    } else if (min != 0) {
        result = QObject::tr("%1 Minutes %2 Seconds").arg(min).arg(sec);
    } else {
        result = QObject::tr("%1 Seconds").arg(sec);
    }
    return result;
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
        ui->btnSuspend->setText(tr("Continue"));
        //TODO: Add logic to pause the download
    }
    else {
        ui->btnSuspend->setText(tr("Pause"));
        //TODO: Add logic to resume the download
    }
}

void DownloadItemWidget::onBtnDeleteClicked()
{
    //TODO:Add Logic to terminate the download
    this->isDownloading = false;

    emit removeFromWidgetRequested(this->listItem);//Request the ListWidget to remove this
}

void DownloadItemWidget::onBtnOpenClicked()
{
    //Open the downloaded file
    QFileInfo fileInfo(savedFilePath, fileName);
    QString savedFilePathName = fileInfo.absoluteFilePath();
    QUrl fileUrl = QUrl::fromLocalFile(savedFilePathName);
    if (!QDesktopServices::openUrl(fileUrl)) {
        QMessageBox::critical(this, tr("Error"),
                              tr("Failed in opening %1 !\nPlease check whether the file exists and the "
                                 "application can access the file.")
                                      .arg(QDir::toNativeSeparators(savedFilePathName)));
        spdlog::error("Failed in opening {} !", QDir::toNativeSeparators(savedFilePathName));
    }
}

void DownloadItemWidget::onBtnMoreClicked() {}

void DownloadItemWidget::iniUi()
{
    //set progress bar ui
    ui->progressBar->setMaximum(qint64_totalBytes);
    ui->progressBar->setValue(qint64_downloadedBytes);

    //display filename
    ui->labFileName->setText(fileName);

    std::pair<double, QString> resultTotal = convertToReasonableUnit(qint64_totalBytes);
    ui->labFileSize->setText(tr("File Size: %1 %2").arg(resultTotal.first, 0, 'f', 2).arg(resultTotal.second));
}

DownloadItemWidget::~DownloadItemWidget()
{
    delete ui;
}
