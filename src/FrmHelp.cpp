#include "FrmHelp.h"
#include "ui_frmhelp.h"

FrmHelp::FrmHelp(QWidget* parent) : QWidget(parent), ui(new Ui::FrmHelp)
{
    ui->setupUi(this);
}

FrmHelp::~FrmHelp()
{
    delete ui;
}
