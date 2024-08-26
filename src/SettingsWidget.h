#pragma once

#include <QWidget>

#include "SettingsAdvancedWidget.h"
#include "SettingsBasicWidget.h"

QT_BEGIN_NAMESPACE
namespace Ui {
    class SettingsWidget;
}
QT_END_NAMESPACE

class QPushButton;

class SettingsBasicWidget;
class SettingsAdvancedWidget;

class SettingsWidget : public QWidget {
    Q_OBJECT

public:
    explicit SettingsWidget(QWidget* parent = nullptr);
    ~SettingsWidget();

    inline void addToTabsList();
    void iniUi();
    void connectSlots();

protected slots:
    void onTabsClicked();
private:
    Ui::SettingsWidget* ui;
    QList<QPushButton*> tabsList;   //存储该选项卡页面的选项卡

    SettingsBasicWidget* settingsBasicWidget;
    SettingsAdvancedWidget* settingsAdvancedWidget;
};
