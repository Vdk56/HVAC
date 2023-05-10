#include "notification3.h"
#include "ui_notification3.h"

notification3::notification3(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::notification3)
{
    ui->setupUi(this);
     this->setWindowFlag(Qt::WindowStaysOnTopHint);
}

notification3::~notification3()
{
    delete ui;
}

void notification3::on_pushButton_clicked()
{
    this->close();
}
