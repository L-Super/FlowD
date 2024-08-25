#include "FrmSettings.h"
#include "ui_frmsettings.h"

FrmSettings::FrmSettings(QWidget* parent) : QWidget(parent), ui(new Ui::FrmSettings)
{
    ui->setupUi(this);
}

FrmSettings::~FrmSettings()
{
    delete ui;
}
