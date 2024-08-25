#pragma once

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
    class FrmSettings;
}
QT_END_NAMESPACE


class FrmSettings : public QWidget {
    Q_OBJECT

public:
    explicit FrmSettings(QWidget* parent = nullptr);
    ~FrmSettings();

private:
    Ui::FrmSettings* ui;
};
