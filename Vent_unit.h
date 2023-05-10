#ifndef VENT_UNIT_H
#define VENT_UNIT_H
#include <QWidget>
#include <QDebug>
#include <QList>
#include <QGraphicsScene>
#include <QPixmap>
#include<QLabel>
#include<QPainter>
#include<QTimer>
#include<QMovie>
#include <QColor>
#include<QImage>
#include<QPaintDevice>
#include "widget_scheme.h"
#include "widget_chart.h"
#include "archive.h"

namespace Ui {
class Vent_unit;
}

class Vent_unit : public QWidget
{
    Q_OBJECT

public:
    explicit Vent_unit(QWidget *parent = nullptr);
    ~Vent_unit();

widget_scheme *ws;
widget_chart *wch;
archive *arh = new archive;
//метод считывает ошибки из архива и заполняет таблицу сообщений
void fill_table_init(int unit);
//номер устройства, для которого открыто окно управления
 int ch_unit;
 //таймер, по сигналу которого испускается сигнал для класса MainWindow для того что бы
 // этот класс отправил сигнал с нужными данными регистров
 QTimer *timer_cycle;
 //изображения зима/лето
 QPixmap zim;
 QPixmap let;
 //структура в которой хранятся ячейчи одной строки таблицы сообщений
 struct items_str{
     QTableWidgetItem *item1 = new QTableWidgetItem;
     QTableWidgetItem *item2 = new QTableWidgetItem;
 };
 //вектор со строками сообщений
 QVector<items_str*> items_v;

public slots:
   //слот для начала отображения окна управления и виджетов внутри него
    void show_unit();
    //отправка сигнала для класса MainWindow что бы получить от него нужные регистры состояний
    void time();
    //этот класс определяет текущее состояние установки и в зависимости от этого окрашивает
    //индикатор состояния (справа от кнопок пуск/стоп) в соответствующий цвет. Так же он регистрирует
    //возникновение новой ошибки или пропадание старой
    void unitStat(bool oper,bool zl, bool locRem, bool fault, int faults);
    // заносит новую ошибку в таблицу и окрашивает фон в красный или окрашивает фон в белый
    //если ошибка стала не активной
    void messages_journ(int faults,int previous);

signals:
    void unit_number(int ch_unit);
    //сигнал испускается из всех слотов кнопок с номером вент установки, значением и индексом слота
    //кнопки из которого он отправлен
    void em(int dev,int type,int value);

    void unitS(int dev);

private slots:


    void on_ustavka_t_valueChanged(int value);

    void on_ustavka_h_valueChanged(int value);

    void on_ustZL_valueChanged(int value);

    void on_pusk_butt_clicked();

    void on_stop_butt_clicked();

    void on_remLoc_butt_clicked();

    void on_reset_butt_clicked();

private:
    Ui::Vent_unit *ui;

};

#endif // VENT_UNIT_H
