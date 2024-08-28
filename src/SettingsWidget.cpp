#include "SettingsWidget.h"
#include "ui_SettingsWidget.h"

#include <QPushButton>
SettingsWidget::SettingsWidget(QWidget* parent) : QWidget(parent), ui(new Ui::SettingsWidget)
{
    ui->setupUi(this);

    addToTabsList();
    iniUi();
    connectSlots();
}

// This function adds the tabs within this tab page to tabsList for easy control
void SettingsWidget::addToTabsList()
{
    // The order of addition cannot be swapped
    tabsList.append(ui->btnTabBasic);
    tabsList.append(ui->btnTabAdvanced);
}

void SettingsWidget::iniUi()
{
    settingsBasicWidget = new SettingsBasicWidget;
    settingsAdvancedWidget = new SettingsAdvancedWidget;
    ui->stackedWidget->insertWidget(0, settingsBasicWidget);
    ui->stackedWidget->insertWidget(1, settingsAdvancedWidget);

    tabsList[0]->setChecked(true);
    ui->stackedWidget->setCurrentIndex(0);
}

void SettingsWidget::connectSlots()
{
    // Connect the slot for tabList
    for (int i = 0; i < tabsList.count(); ++i)
        connect(tabsList[i], &QPushButton::clicked, this, &SettingsWidget::onTabsClicked);
}

void SettingsWidget::onTabsClicked()
{
    QPushButton* btnTabClicked = qobject_cast<QPushButton*>(sender());
    // Find the pressed tab
    for (int i = 0; i < tabsList.count(); ++i) {
        if (tabsList[i] == btnTabClicked) {
            ui->stackedWidget->setCurrentIndex(i);
            if (!tabsList[i]->isChecked())
                tabsList[i]->setChecked(true);
        }
        else {
            tabsList[i]->setChecked(false);
        }
    }
}

SettingsWidget::~SettingsWidget()
{
    delete ui;
}
