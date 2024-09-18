#pragma once

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
    class SettingsWidget;
}
QT_END_NAMESPACE

class SettingsBasicWidget;
class SettingsAdvancedWidget;

class SettingsWidget : public QWidget {
    Q_OBJECT

public:
    explicit SettingsWidget(QWidget* parent = nullptr);
    ~SettingsWidget() override;

private:
    Ui::SettingsWidget* ui;
    SettingsBasicWidget* basicWidget;
    SettingsAdvancedWidget* advancedWidget;
};
