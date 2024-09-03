#pragma once

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
    class FileInformationWidget;
}
QT_END_NAMESPACE

class FileInformationWidget : public QWidget {
    Q_OBJECT

public:
    explicit FileInformationWidget(QString FileName, QString SavedFilePath, QString URL, qint64 FileSize,
                                   QString Status, QWidget* parent = nullptr);
    ~FileInformationWidget() override;

private:
    Ui::FileInformationWidget* ui;
    QString fileName;
    QString savedFilePath;
    QString url;
    QString status;
    qint64 fileSize;

    void iniUi();
    void connectSlots();
};
