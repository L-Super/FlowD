#pragma once

#include <QWidget>

namespace Ui {
    class SettingsAdvancedWidget;
}

class SettingsAdvancedWidget : public QWidget {
    Q_OBJECT

public:
    explicit SettingsAdvancedWidget(QWidget* parent = nullptr);
    ~SettingsAdvancedWidget();

private:
    Ui::SettingsAdvancedWidget* ui;
};
