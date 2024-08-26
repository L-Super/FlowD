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

protected slots:

private:
    Ui::NewDownloadDialog* ui;

    void connectSlots();
    void resizeWidget(bool minMode);//调整窗口大小
};
