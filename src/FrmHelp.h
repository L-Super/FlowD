#pragma once

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
    class FrmHelp;
}
QT_END_NAMESPACE


class FrmHelp : public QWidget {
    Q_OBJECT

public:
    explicit FrmHelp(QWidget* parent = nullptr);
    ~FrmHelp() override;

private:
    Ui::FrmHelp* ui;
};
