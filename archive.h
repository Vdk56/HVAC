#ifndef ARCHIVE_H
#define ARCHIVE_H

#include <QObject>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>

class archive
{
public:
    archive();
    //Функция создаёт файл архива устройства и пишет туда время и значения измеряемых величин
    void write_file(QVector<QString> data,int dev);
    // Эти три метода пишут в файл конфига коды конфигурации вент установки
    void write_scada_cod(QVector<quint32> code);
    void write_sens_cod(QVector<quint32> code);
    void write_cpu_cod(QVector<quint32> code);
    //Метод пишет архив ошибок
    void write_fault_message(QString message, int dev);

    QFile *conf;
};

#endif // ARCHIVE_H
