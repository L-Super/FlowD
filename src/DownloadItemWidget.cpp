#include "DownloadItemWidget.h"
#include "ui_DownloadItemWidget.h"

#include <QDesktopServices>
#include <QDir>
#include <QFileInfo>
#include <QListWidgetItem>
#include <QMessageBox>
#include <QUrl>

#include "Logger.hpp"
#include "logic/DownloadManager.h"

namespace {
std::string convertBytesToReadable(double bytes)
{
    const std::array<std::string, 5> units = {"Bytes", "KB", "MB", "GB", "TB"};

    int i = 0;
    // 将字节转换为更大的单位
    while (bytes >= 1024 && i < static_cast<int>(units.size()) - 1) {
        bytes /= 1024;
        i++;
    }

    return fmt::format("{:.2f} {}", bytes, units[i]);
}

int convertProgressValue(unsigned long current, unsigned long total, int maxVal = 100)
{
    if (total == 0 || maxVal == 0) {
        // 避免除以零
        return 0;
    }

    // 计算进度的比例
    double ratio = static_cast<double>(current) / total;

    // 将比例映射到进度条的最大值
    int mappedValue = static_cast<int>(ratio * maxVal);

    // 确保不会超出进度条的最大值
    if (mappedValue > maxVal) {
        return maxVal;
    }

    return mappedValue;
}

std::string convertDownloadSpeed(unsigned long bytesPerSecond)
{
    double speed = static_cast<double>(bytesPerSecond);
    std::string unit = "B/s";

    if (speed >= 1024 * 1024) {
        // 转换为MB/s
        speed /= 1024 * 1024;
        unit = "MB/s";
    }
    else if (speed >= 1024) {
        // 转换为KB/s
        speed /= 1024;
        unit = "KB/s";
    }

    return fmt::format("{:.2f} {}", speed, unit);
}

std::string convertRemainingTime(double seconds)
{
    if (seconds >= 60) {
        double minutes = seconds / 60;
        return fmt::format("{:.2f} min", minutes);
    }
    else {
        return fmt::format("{:.2f} s", seconds);
    }
}
} // namespace

DownloadItemWidget::DownloadItemWidget(size_t id, QWidget* parent)
    : QWidget(parent), ui(new Ui::DownloadItemWidget), taskID(id), progressMaximum(100)
{
    ui->setupUi(this);

    ui->progressBar->setMaximum(progressMaximum);
    ui->progressBar->setMinimum(0);
    ui->pauseButton->setProperty("isPause", "false");
    connect(ui->pauseButton, &QPushButton::clicked, this, &DownloadItemWidget::onPauseButtonClicked);
    connect(ui->deleteButton, &QPushButton::clicked, this, &DownloadItemWidget::onDeleteButtonClicked);
    connect(ui->openFileButton, &QPushButton::clicked, this, &DownloadItemWidget::onOpenFileButtonClicked);
    connect(ui->moreInfoButton, &QPushButton::clicked, this, &DownloadItemWidget::onMoreInfoButtonClicked);
    connect(this, &DownloadItemWidget::progressUpdateFromTaskSignal, this, &DownloadItemWidget::onProgressUpdate,
            Qt::QueuedConnection);
    connect(this, &DownloadItemWidget::completeDownloadFromTaskSignal, this, &DownloadItemWidget::onCompleteDownload,
            Qt::QueuedConnection);
}

DownloadItemWidget::~DownloadItemWidget()
{
    spdlog::debug(__func__);
    delete ui;
}

void DownloadItemWidget::setDownloadTaskID(size_t id)
{
    taskID = id;
}

size_t DownloadItemWidget::downloadTaskID() const
{
    return taskID;
}

void DownloadItemWidget::setPauseButtonVisible(bool visible)
{
    ui->pauseButton->setVisible(visible);
}

void DownloadItemWidget::setDownloadItemInfo(const DownloadItemInfo& info)
{
    fileInfo = info;
    ui->fileNameLabel->setText(QString::fromStdString(fileInfo.filename));
    auto fileSizeStr = convertBytesToReadable(fileInfo.totalBytes);
    ui->fileSizeLabel->setText(QString::fromStdString(fileSizeStr));
}

const DownloadItemInfo& DownloadItemWidget::downloadItemInfo() const
{
    return fileInfo;
}

void DownloadItemWidget::onPauseButtonClicked(bool /*checked*/)
{
    // TODO: change icon
    if (ui->pauseButton->property("isPause").toBool()) {
        ui->pauseButton->setText(tr("Continue"));
        ui->pauseButton->setProperty("isPause", false);
        DownloadManager::instance().resumeTask(taskID);
    }
    else {
        ui->pauseButton->setText(tr("Pause"));
        ui->pauseButton->setProperty("isPause", true);
        ui->speedLabel->clear();
        ui->remainTimeLabel->clear();
        DownloadManager::instance().pauseTask(taskID);
    }
}

void DownloadItemWidget::onDeleteButtonClicked()
{
    // TODO: confirm weather to remove the file
    DownloadManager::instance().removeTask(taskID);

    // Request the ListWidget to remove this
    emit removeItemSignal();
}

void DownloadItemWidget::onOpenFileButtonClicked()
{
    QFileInfo info(QString::fromStdString(fileInfo.filenamePath));
    QString savedFilePathName = info.absoluteFilePath();
    QUrl fileUrl = QUrl::fromLocalFile(savedFilePathName);
    if (!QDesktopServices::openUrl(fileUrl)) {
        QMessageBox::critical(this, tr("Error"),
                              tr("Failed in opening %1 !\nPlease check whether the file exists and the "
                                 "application can access the file.")
                                  .arg(QDir::toNativeSeparators(savedFilePathName)));
        spdlog::error("Failed to open the file: {}", savedFilePathName);
    }
}

void DownloadItemWidget::onMoreInfoButtonClicked() {}

void DownloadItemWidget::onProgressUpdate(unsigned long total, unsigned long downloaded, unsigned long speed,
                                          double remainTime)
{
    if (downloaded == 0)
        return;
    if (fileInfo.totalBytes == 0) {
        fileInfo.totalBytes = total;
        auto fileSizeStr = convertBytesToReadable(fileInfo.totalBytes);
        ui->fileSizeLabel->setText(QString::fromStdString(fileSizeStr));
    }
    ui->progressBar->setValue(convertProgressValue(downloaded, total));
    ui->speedLabel->setText(QString::fromStdString(convertDownloadSpeed(speed)));
    ui->remainTimeLabel->setText(QString::fromStdString(convertRemainingTime(remainTime)));
}

void DownloadItemWidget::onCompleteDownload()
{
    ui->pauseButton->hide();
    ui->progressBar->setValue(100);

    emit completeDownloadSignal();

    // when complete download, clean the task
    DownloadManager::instance().removeTask(taskID);
}
