#include "SettingsAdvancedWidget.h"
#include "ui_SettingsAdvancedWidget.h"

SettingsAdvancedWidget::SettingsAdvancedWidget(QWidget* parent) : QWidget(parent), ui(new Ui::SettingsAdvancedWidget)
{
    ui->setupUi(this);
}

SettingsAdvancedWidget::~SettingsAdvancedWidget()
{
    delete ui;
}
