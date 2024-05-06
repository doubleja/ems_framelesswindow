#include "pcs.h"
#include "ui_pcs.h"

PCS::PCS(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PCS)
{
    ui->setupUi(this);
}

PCS::~PCS()
{
    delete ui;
}
