#pragma once

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
    class FrmDownloadList;
}
QT_END_NAMESPACE

class FrmDownloadList : public QWidget {
    Q_OBJECT

public:
    explicit FrmDownloadList(QWidget* parent = nullptr);
    ~FrmDownloadList() override;

private:
    Ui::FrmDownloadList* ui;
};
