#include "FrmHelp.h"
#include "ui_FrmHelp.h"

FrmHelp::FrmHelp(QWidget* parent) : QWidget(parent), ui(new Ui::FrmHelp)
{
    ui->setupUi(this);
}

FrmHelp::~FrmHelp()
{
    delete ui;
}
