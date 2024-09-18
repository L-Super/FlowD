#include "SettingsWidget.h"
#include "ui_SettingsWidget.h"

#include "SettingsAdvancedWidget.h"
#include "SettingsBasicWidget.h"

SettingsWidget::SettingsWidget(QWidget* parent)
    : QWidget(parent), ui(new Ui::SettingsWidget), basicWidget(new SettingsBasicWidget(this)),
      advancedWidget(new SettingsAdvancedWidget(this))
{
    ui->setupUi(this);

    ui->stackedWidget->insertWidget(0, basicWidget);
    ui->stackedWidget->insertWidget(1, advancedWidget);
    ui->stackedWidget->setCurrentIndex(0);

    ui->basicTabButton->setChecked(true);

    connect(ui->basicTabButton, &QPushButton::clicked, this, [this] {
        ui->stackedWidget->setCurrentIndex(0);
        ui->basicTabButton->setChecked(true);
        ui->advanceTabButton->setChecked(false);
    });
    connect(ui->advanceTabButton, &QPushButton::clicked, this, [this] {
        ui->stackedWidget->setCurrentIndex(1);
        ui->basicTabButton->setChecked(false);
        ui->advanceTabButton->setChecked(true);
    });
}

SettingsWidget::~SettingsWidget()
{
    delete ui;
}
