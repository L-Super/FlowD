#pragma once

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
    class DownloadItemWidget;
}
QT_END_NAMESPACE

class DownloadItemWidget : public QWidget {
    Q_OBJECT

public:
    explicit DownloadItemWidget(QWidget* parent = nullptr);
    ~DownloadItemWidget() override;

    void setFileName(QString fileName);
    void setSavedFilePath(QString filePath);
    void setFileUrl(QString fileUrl);
    void setFileDownloadProgress(qint64 totalBytes, qint64 downloadedBytes);
    void setDownloadState(bool isDownloading);
    void setRemainTime(qint64 remainTime);
    void setDownloadSpeed(double downloadSpeed);

    QString getFileName();
    QString getSavedFilePath();
    QString getFileUrl();

    void getFileDownloadProgress(qint64& totalBytes, qint64& downloadedBytes);

    bool getDownloadState();

    qint64 getRemainTime();

    double getDownloadSpeed();

protected slots:
    void onBtnSuspendClicked(bool checked);

private:
    Ui::DownloadItemWidget* ui;

    bool isDownloading;

    qint64 qint64_downloadedBytes;
    qint64 qint64_totalBytes;
    qint64 qint64_remainTime;

    double double_downloadSpeed;

    QString fileName;
    QString fileUrl;
    QString savedFilePath;


    void connectSlots();
};
