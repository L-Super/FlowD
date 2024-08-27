#pragma once

#include <QWidget>

namespace Ui {
    class SettingsBasicWidget;
}

class SettingsBasicWidget : public QWidget {
    Q_OBJECT

public:
    explicit SettingsBasicWidget(QWidget* parent = nullptr);
    ~SettingsBasicWidget();

private:
    Ui::SettingsBasicWidget* ui;
};
