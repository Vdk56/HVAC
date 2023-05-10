#ifndef WIDGET_CHART_H
#define WIDGET_CHART_H

#include <QWidget>
#include<QObject>
#include <QtCharts>
#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>
#include <QtCharts/QSplineSeries>
#include <QFile>
#include<QPainter>
#include<QAbstractAxis>
#include<QTimer>
#include<QDateTime>
#include "nitification1.h"
#include "notification2.h"



namespace Ui {
class widget_chart;
}

class widget_chart : public QWidget
{
    Q_OBJECT

public:
    explicit widget_chart(QWidget *parent = nullptr);
    ~widget_chart();

    //данные из этого вектора получают при вызове методов get_list, get_list_for_goButton и get_list_for_compare_Button
    // в нём хранятся знчения величин за определённый привызове методов период
    QVector<QStringList> list_to_chart;
    QVector<QStringList> get_list(int start, int end);
    QVector<QStringList> get_list_for_goButton(int start, int end);
    QVector<QStringList> get_list_for_compare_Button(int start, int end,int ser);
    //в эти векторы загружаются данные из вектора list_to_chart при вызове метода get_list_for_compare_Button
    QVector<QStringList> compare_vector_1;
    QVector<QStringList> compare_vector_2;

    //это значение, на которое мы умножаем значение интервала при сравнении. Его нужно настраивать
    //в зависимости от того как часто данные пишутся в архив
    int interval_MINx5SEC_compare;
    //данные о количестве строк в файле для навигации по графику
    int strings_amount();
    //метод отрисовки графика в режиме записи
    void draw_chart();
    //метод который возвращает количество строк, нужен для режима архива и сравнения
    int get_string_number(QString d);
    QTimer *t;
    QTimer *tu;
    quint32 erliest_date();
    bool eventFilter(QObject *target, QEvent *event);

    QGraphicsScene *scene1 = new QGraphicsScene;

    QChartView view;
    QValueAxis *axisY=new  QValueAxis;
    QDateTimeAxis *axisX = new QDateTimeAxis;
    QDateTimeAxis *axisXX=new QDateTimeAxis;
    QDateTimeAxis *axisXcomp;
    QDateTimeAxis *axisXXcomp;
    //переменная определяющая можно ли отображать доп.оси. Нужна для перехода в режим сравнения и назад
    bool extra_axes=0;
   //объекты для отрисовки поля графиков и самих графиков
    QChart *chart=new QChart;
    QLineSeries *series1 = new QLineSeries;
    QLineSeries *series2 = new QLineSeries;
    QLineSeries *series3 =new QLineSeries;
    QLineSeries *series4  =new QLineSeries;
public slots:

    void chart_refresh();
     void set_dev(int i);
    void slotGetM(int t11,int t22,int t33, int t44);
    void hover_ser(QPointF p, bool state);
    void timer_un();
signals:
   void  signalGetM(int unit);
private slots:

   void on_arch_rec_b_clicked();

   void on_record_but_clicked();

   void on_checkBox_stateChanged(int arg1);

   void on_checkBox_2_stateChanged(int arg1);

   void on_checkBox_3_stateChanged(int arg1);

   void on_checkBox_4_stateChanged(int arg1);

   void on_compare_but_clicked();

   void on_back_but_clicked();

   void on_forw_but_clicked();

   void on_go_to_but_clicked();

private:
    Ui::widget_chart *ui;
    QFile *file;
    nitification1 *notify=new nitification1;
    notification2 *notify1 = new notification2;
};

#endif // WIDGET_CHART_H
