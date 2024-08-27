#include "SettingsBasicWidget.h"
#include "ui_SettingsBasicWidget.h"

SettingsBasicWidget::SettingsBasicWidget(QWidget* parent) : QWidget(parent), ui(new Ui::SettingsBasicWidget)
{
    ui->setupUi(this);
}

SettingsBasicWidget::~SettingsBasicWidget()
{
    delete ui;
}
