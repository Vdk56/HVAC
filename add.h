#ifndef ADD_H
#define ADD_H

#include <QWidget>
#include <QTableWidgetItem>
#include <QComboBox>
#include <QLineEdit>
#include <QDebug>
#include <QWidget>
#include <QFile>
#include <QTextStream>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
namespace Ui {
class ADD;
}

class ADD : public QWidget
{
    Q_OBJECT

public:
    explicit ADD(QWidget *parent = nullptr);
    ~ADD();
    struct  dev_info
    {
        QString dev_name;
        QString dev_ip;
        QString dev_port;
        QString mod_ad;
    };
         dev_info dev;
         QIntValidator *val;
         QFile *un_data;
         void write_JSON();
private slots:

    void on_cancel_but_clicked();

    void on_done_but_clicked();
signals:
    void refresh();
private:
    Ui::ADD *ui;
};

#endif // ADD_H
