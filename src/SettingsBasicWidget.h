#pragma once

#include <QWidget>
#include <QHash>
QT_BEGIN_NAMESPACE
namespace Ui {
    class SettingsBasicWidget;
}
QT_END_NAMESPACE

class QSettings;

class SettingsBasicWidget : public QWidget {
    Q_OBJECT

public:
    explicit SettingsBasicWidget(QWidget* parent = nullptr);
    ~SettingsBasicWidget() override;

    void addToChangedHash(QString settingsPath,QVariant value);

private:
    Ui::SettingsBasicWidget* ui;
    QHash<QString,QVariant> changedSettings;
    QSettings* set;

    void iniSettings();
    void connectSlots();

};
