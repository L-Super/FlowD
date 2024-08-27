#pragma once

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
    class SettingsBasicWidget;
}
QT_END_NAMESPACE

class SettingsBasicWidget : public QWidget {
    Q_OBJECT

public:
    explicit SettingsBasicWidget(QWidget* parent = nullptr);
    ~SettingsBasicWidget() override;

private:
    Ui::SettingsBasicWidget* ui;
};
