#pragma once

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
    class DownloadItemWidget;
}
QT_END_NAMESPACE

class QListWidgetItem;

class DownloadItemWidget : public QWidget {
    Q_OBJECT

public:
    explicit DownloadItemWidget(QString URL, QString fileName, QString fileSavedPath, qint64 totalBytes,
                                qint64 downloadedBytes, bool isDownloading, QListWidgetItem* listItem,
                                QWidget* parent = nullptr);
    ~DownloadItemWidget() override;

    void setFileName(QString fileName);
    void setSavedFilePath(QString filePath);
    void setFileUrl(QString fileUrl);
    void setFileDownloadProgress(qint64 totalBytes, qint64 downloadedBytes);
    void setDownloadState(bool isDownloading);
    void setRemainTime();
    void setDownloadSpeed(double downloadSpeed);

    std::pair<double, QString> convertToReasonableUnit(double bytesToConvert);
    QString convertToReasonableTimeUnit(qint64 sec,qint64 min=0,qint64 hour=0,qint64 day=0);

    QString getFileName();
    QString getSavedFilePath();
    QString getFileUrl();

    void getFileDownloadProgress(qint64& totalBytes, qint64& downloadedBytes);

    bool getDownloadState();

    qint64 getRemainTime();

    double getDownloadSpeed();

protected slots:
    void onBtnSuspendClicked(bool checked);

    void onBtnDeleteClicked();

    void onBtnOpenClicked();

    void onBtnMoreClicked();

private:
    Ui::DownloadItemWidget* ui;

    QListWidgetItem* listItem;

    bool isDownloading;

    qint64 qint64_downloadedBytes;
    qint64 qint64_totalBytes;
    qint64 qint64_remainTime;

    double double_downloadSpeed;

    QString fileName;
    QString fileUrl;
    QString savedFilePath;


    void iniUi();
    void connectSlots();

signals:
    void removeFromWidgetRequested(QListWidgetItem* item);
};
