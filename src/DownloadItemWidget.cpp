#include "DownloadItemWidget.h"
#include "ui_DownloadItemWidget.h"

#include <QDesktopServices>
#include <QDir>
#include <QFileInfo>
#include <QListWidgetItem>
#include <QMessageBox>
#include <QUrl>

#include "Logger.hpp"
#include "Logic/DownloadManager.h"

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
}// namespace


DownloadItemWidget::DownloadItemWidget(size_t id, QWidget* parent)
    : QWidget(parent), ui(new Ui::DownloadItemWidget), taskID(id)
{
    ui->setupUi(this);

    auto taskInfo = DownloadManager::instance().downloadTaskInfo(taskID);

    if (taskInfo.has_value()) {
        fileInfo = taskInfo.value();
        //TODO: get info from DownloadManager
        ui->fileNameLabel->setText(QString::fromStdString(fileInfo.filename));
        auto fileSizeStr = convertBytesToReadable(fileInfo.totalBytes);
        ui->fileSizeLabel->setText(QString::fromStdString(fileSizeStr));
        ui->progressBar->setMaximum(fileInfo.totalBytes);
    }
    else {
        spdlog::warn("Get null file info from DownloadManager");
    }

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
    spdlog::debug(__func__ );
    delete ui;
}

size_t DownloadItemWidget::downloadTaskID() const
{
    return taskID;
}

void DownloadItemWidget::hidePauseButton(bool hide)
{
    ui->pauseButton->setHidden(hide);
}

void DownloadItemWidget::onPauseButtonClicked(bool /*checked*/)
{
    //TODO: change icon
    if (ui->pauseButton->property("isPause").toBool()) {
        ui->pauseButton->setText(tr("Continue"));
        ui->pauseButton->setProperty("isPause", false);
        DownloadManager::instance().resumeTask(taskID);
    }
    else {
        ui->pauseButton->setText(tr("Pause"));
        ui->pauseButton->setProperty("isPause", true);
        DownloadManager::instance().pauseTask(taskID);
    }
}

void DownloadItemWidget::onDeleteButtonClicked()
{
    //TODO: remove it from the download list and confirm weather to remove the file
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
        spdlog::error("Failed in opening {} !", savedFilePathName);
    }
}

void DownloadItemWidget::onMoreInfoButtonClicked() {}

void DownloadItemWidget::onCompleteDownload()
{
    ui->pauseButton->hide();
    //TODO: add complete download logic and remove task

    //TODO: when download complete, remove it from downloading list, and then add it into finished list
    emit completeDownloadSignal();
}

void DownloadItemWidget::onProgressUpdate(unsigned long total, unsigned long downloaded)
{
    //TODO:
    // ui->speedLabel->setText(QString("%1 MB/s").arg(1));
    // ui->remainTimeLabel
    if (downloaded == 0)
        return;
    ui->progressBar->setValue(downloaded);
}
