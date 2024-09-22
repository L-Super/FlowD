#pragma once

#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui {
    class NewDownloadDialog;
}
QT_END_NAMESPACE

class NewDownloadDialog : public QDialog {
    Q_OBJECT

public:
    explicit NewDownloadDialog(QWidget* parent = nullptr);
    ~NewDownloadDialog() override;

    QString downloadUrl();
    QString saveFilePath();
    unsigned int threadCount();

protected slots:
    void showAdvanceOption(bool minMode);

private:
    Ui::NewDownloadDialog* ui;
    QString url;
};
