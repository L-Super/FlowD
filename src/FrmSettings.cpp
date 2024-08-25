#include "FrmSettings.h"
#include "ui_FrmSettings.h"

FrmSettings::FrmSettings(QWidget* parent) : QWidget(parent), ui(new Ui::FrmSettings)
{
    ui->setupUi(this);
}

FrmSettings::~FrmSettings()
{
    delete ui;
}
