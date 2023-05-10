#include "autorization.h"
#include "ui_autorization.h"

autorization::autorization(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::autorization)
{
    ui->setupUi(this);
    this->setWindowFlag(Qt::WindowStaysOnTopHint);
    Qt::WindowFlags flags = windowFlags();
    Qt::WindowFlags closeFlag = Qt::WindowCloseButtonHint;
    flags = flags & (~closeFlag);
    setWindowFlags(flags);
}

autorization::~autorization()
{
    delete ui;
}

void autorization::on_pushButton_clicked()
{
    if((ui->lineEdit->text()=="admin")&(ui->lineEdit_2->text()=="admin")){
        status = 1;
        emit logi(status);
        this->close();
    }
    else if((ui->lineEdit->text()=="user")&(ui->lineEdit_2->text()=="user")){
        status = 2;
        emit logi(status);
        this->close();
    }
    else n.show();
}

void autorization::on_pushButton_2_clicked()
{
    status = 3;
    emit logi(status);
    this->close();
}
