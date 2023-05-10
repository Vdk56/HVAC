#include "add.h"
#include "ui_add.h"

ADD::ADD(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ADD)
{
    ui->setupUi(this);

}

ADD::~ADD()
{
    delete ui;
}

void ADD::on_done_but_clicked()
{
    dev.dev_name = ui->dev_name_l->text();
    dev.dev_ip = ui->dev_ip_l->text();
    dev.dev_port = ui->dev_port_l->text();
    dev.mod_ad = ui->modb_adr_l->text();
  
    write_JSON();
    emit  refresh();
    this->hide();
}

void ADD::on_cancel_but_clicked()
{
    this->close();
}


void ADD::write_JSON()
{
    un_data = new QFile("config.cfg");
    un_data->open(QIODevice::ReadOnly);
    QString content;
    for(;!un_data->atEnd();) content = content + un_data->readLine();
    QJsonDocument jdoc = QJsonDocument::fromJson(content.toUtf8());
    QJsonObject sub_object;
    QJsonObject jobj = jdoc.object();
    un_data->close();
    QJsonArray arr_temp = jobj["Devices"].toArray();

    ub_object["Name:"] =dev.dev_name;
    sub_object["Address_ip:"]=dev.dev_ip;
    sub_object["Port:"]=dev.dev_port;
    sub_object["Modbus Address:"]=dev.mod_ad;
    sub_object["Device_config"]="";
    sub_object["Sensors_config"]="";

    arr_temp.push_back(sub_object);

    jobj["Devices"]=arr_temp;

    QByteArray b_a = QJsonDocument(jobj).toJson();
    un_data->open(QIODevice::WriteOnly);
    un_data->write(b_a);
    un_data->close();
}

