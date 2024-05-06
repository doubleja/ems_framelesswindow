#include "pcs.h"
#include "ui_pcs.h"
#include <QDebug>
PCS::PCS(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PCS)
{
    ui->setupUi(this);
    connect(ui->pushButton,&QPushButton::clicked,this,&PCS::myslots);
}

PCS::~PCS()
{
    delete ui;
}


void PCS::myslots()
{
    qDebug()<<"myslots";

    // 获取当前时间
    QDateTime currentTime = QDateTime::currentDateTime();

    // 使用 Http 对象进行操作

    http.onenetRetData("1097949264",currentTime);

}
