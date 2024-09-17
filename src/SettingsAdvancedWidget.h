#pragma once

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
    class SettingsAdvancedWidget;
}
QT_END_NAMESPACE

class SettingsAdvancedWidget : public QWidget {
    Q_OBJECT

public:
    explicit SettingsAdvancedWidget(QWidget* parent = nullptr);
    ~SettingsAdvancedWidget() override;

private:
    Ui::SettingsAdvancedWidget* ui;
};
