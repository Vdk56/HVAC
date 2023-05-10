#include "nitification1.h"
#include "ui_nitification1.h"

nitification1::nitification1(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::nitification1)
{
    ui->setupUi(this);
}

nitification1::~nitification1()
{
    delete ui;
}

void nitification1::on_pushButton_clicked()
{
    this->hide();
}
