#pragma once

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
    class SettingsWidget;
}
QT_END_NAMESPACE


class SettingsWidget : public QWidget {
    Q_OBJECT

public:
    explicit SettingsWidget(QWidget* parent = nullptr);
    ~SettingsWidget();

private:
    Ui::SettingsWidget* ui;
};
