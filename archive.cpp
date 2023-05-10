#include "archive.h"

archive::archive()
{

}

void archive::write_file(QVector<QString> data,int dev)
{
    QStringList d;
    QString temp = "";
    for(int i = 0;i<data.count();i++){
        temp.append(data[i]);
    }
    QString file_name = "archive_"+QString::number(dev)+".txt";
    QFile file(file_name);
    QTextStream ts( &file);
    file.open(QIODevice::ReadWrite);
    file.readAll();
    ts<<temp<<"\n";
    file.close();
}

void archive::write_scada_cod(QVector<quint32> code)
{
    conf = new QFile("config.cfg");
    conf->open(QIODevice::ReadOnly);
    QString content;
    for(;!conf->atEnd();) content = content + conf->readLine();
    QJsonDocument jdoc = QJsonDocument::fromJson(content.toUtf8());
    QJsonObject jobj = jdoc.object();

    conf->close();
    QJsonArray arr_temp = jobj["Devices"].toArray();
    for(int i = 0;i<arr_temp.count();i++)
    {
           QJsonObject obj_code = arr_temp[i].toObject();
           obj_code["Device_config"]=QString::number(code[i]);
           arr_temp[i] = obj_code;
    }

jobj["Devices"]=arr_temp;
    QByteArray b_a = QJsonDocument(jobj).toJson();
    conf->open(QIODevice::WriteOnly);
    conf->write(b_a);
    conf->close();

}

void archive::write_sens_cod(QVector<quint32> code)
{
    conf = new QFile("config.cfg");
    conf->open(QIODevice::ReadOnly);
    QString content;
    for(;!conf->atEnd();) content = content + conf->readLine();
    QJsonDocument jdoc = QJsonDocument::fromJson(content.toUtf8());
    QJsonObject jobj = jdoc.object();
    QJsonObject obj_code;
    conf->close();
    QJsonArray arr_temp = jobj["Devices"].toArray();
    for(int i = 0;i<code.count();i++)
    {
            QJsonObject obj_code=arr_temp[i].toObject();
            obj_code["Sensors_config"]=QString::number(code[i]);
            arr_temp[i] = obj_code;
    }
    jobj["Devices"]=arr_temp;

    QByteArray b_a = QJsonDocument(jobj).toJson();
    conf->open(QIODevice::WriteOnly);
    conf->write(b_a);
    conf->close();
}

void archive::write_cpu_cod(QVector<quint32> code)
{
    conf = new QFile("config.cfg");
    conf->open(QIODevice::ReadOnly);
    QString content;
    for(;!conf->atEnd();) content = content + conf->readLine();
    QJsonDocument jdoc = QJsonDocument::fromJson(content.toUtf8());
    QJsonObject jobj = jdoc.object();
    QJsonObject obj_code;
    conf->close();
    QJsonArray arr_temp = jobj["Devices"].toArray();
    for(int i = 0;i<code.count();i++)
    {
            QJsonObject obj_code=arr_temp[i].toObject();
            obj_code["CPU_config"]=QString::number(code[i]);
            arr_temp[i] = obj_code;
    }
    jobj["Devices"]=arr_temp;
    QByteArray b_a = QJsonDocument(jobj).toJson();
    conf->open(QIODevice::WriteOnly);
    conf->write(b_a);
    conf->close();
}

void archive::write_fault_message(QString message, int dev)
{
    QString file_name = "messages_"+QString::number(dev)+".txt";
    QFile file(file_name);
    QTextStream ts( &file);
    file.open(QIODevice::ReadWrite);
    file.readAll();
    ts<<message<<"\n";
    file.close();
}
