#include "notification2.h"
#include "ui_notification2.h"

notification2::notification2(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::notification2)
{
    ui->setupUi(this);
}

notification2::~notification2()
{
    delete ui;
}

void notification2::on_pushButton_clicked()
{
    this->hide();
}
