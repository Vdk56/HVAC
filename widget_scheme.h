#ifndef WIDGET_SCHEME_H
#define WIDGET_SCHEME_H

#include <QWidget>
#include <QWidget>
#include <QDebug>
#include <QList>
#include <QGraphicsScene>
#include <QPixmap>
#include<QLabel>
#include <QFile>
#include<QPainter>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include<QTimer>
#include<QMovie>
#include<QDebug>
#include<QImage>
#include<QPaintDevice>

namespace Ui {
class widget_scheme;
}
class widget_scheme : public QWidget
{
    Q_OBJECT
public:
    explicit widget_scheme(QWidget *parent = nullptr);
    ~widget_scheme();
    QTimer *timer_cycle_paint_ev;
    QTimer timer_signal;
    //объектры для изображений формата gif
    QMovie fun_input_gif;
    QMovie fun_output_gif;
    QMovie frege_pump_stop_gif;
    QMovie jalusi_input_stop_gif;
    QMovie nasos_run_gif;
    QMovie oros_h_start_gif;
    QMovie rotor_ut_start_gif;
    QMovie steam_h_start_gif;
    QMovie TEN_cooled_gif;
    QMovie vod_kal_start_gif;
    QMovie Arr_gif;
    QMovie air_gif;
    QMovie coller_nasos_runKal1;
    QMovie coller_nasos_runKal2;
    QMovie coller_nasos_run;

    //коды состояний для управления объектами изображений
    int SCADA_CODE;
    int SENS_CODE;
    int CPU_CODE;
    int ALARM_CODE;

    //Объектры для изображений
    QImage odin;
    QImage tube;
    QImage coller_pump_al;
    QImage coller_not_used;
    QImage coller_not_used_pump;
    QImage coller_start_1;
    QImage coller_start_2;
    QImage coller_start_3;
    QImage coller_stop;
    QImage coller_stop_pump;
    QImage fan_inp;
    QImage fan_inp_vfd;
    QImage fan_input_vfderr;
    QImage fan_input_remen;
    QImage fan_input_thermo;
    QImage fan_outp;
    QImage fan_out_rem_err;
    QImage fan_out_termoerr;
    QImage fan_out_vfd;
    QImage fan_output_vfderr;
    QImage fan_output_remen;
    QImage fan_output_thermo;
    QImage filter_in;
    QImage filter_in_al_start;
    QImage filter_in_al_stop;
    QImage filter_in_ok_stop;
    QImage filter_out;
    QImage filter_out_al;
    QImage filter_out_al_stop;
    QImage filter_output_ok_stop;
    QImage frege_pump_stop;
    QImage jaluzi_inp_start;
    QImage jalusi_input_stop;
    QImage jaluzi_in_err;
    QImage jaluzi_free_stop;
    QImage jaluzi_in_heat;
    QImage jaluzi_inp_sens_op;
    QImage jaluzi_inp_sens_op_err;
    QImage jaluzi_inp_sens_close;
    QImage jaluzi_inp_sens_close_err;
    QImage oros_hum_start;
    QImage plast_util_al;
    QImage plast_util_not_used;
    QImage plast_util_stop;
    QImage plast_util_freeze;
    QImage rotor_util;
    QImage rotor_util_not_use;
    QImage TEN_alarm;
    QImage TEN_cooled;
    QImage TEN_not_used;
    QImage TEN_start1;
    QImage TEN_start2;
    QImage TEN_start3;
    QImage Ten1_dis;
    QImage TEN_stoped;
    QImage Winter;
    QImage Summer;
    QImage bypass_open;
    QImage bypass_close;
    QImage Vod_Kal_alarm;
    QImage Vod_Kal_no_water;
    QImage Vod_Kal_not_used;
    QImage Vod_Kal_start1;
    QImage Vod_Kal_start2;
    QImage Vod_Kal_start3;
    QImage Vod_Kal_stop;
    QImage oros_uvlaz_alarm;
    QImage oros_uvlaz_not_used;
    QImage oros_uvlaz_stop;
    QImage steam_hum_start;
    QImage steam_hum_alarm;
    QImage steam_hum_not_used;
    QImage steam_hum_stop;
    QImage recyrc_util_alarm;
    QImage recyrc_util_not_used;
    QImage recyrc_util_start;
    QImage recyrc_util_stop;
    QImage rotor_util_alarm;
    QImage rotor_util_freeze;
    QImage rotor_util_not_used;
    QImage freon_oxl_alarm;
    QImage freon_oxl_freeze;
    QImage freon_oxl_not_used;
    QImage freon_oxl_pusk;
    QImage freon_oxl_stop;
    QImage wall_outside;
    QImage wall_inside;

    //объект QFile для чтения конфига устройства
    QFile *f;
    void paintEvent(QPaintEvent *event);
    //инициализация в соответствии с кодами конфигурации
    void init_pics(int scada_code,  int sens_code, int cpu_code);
signals:
     //сигнал с номером вент установки для получения данных о состоянии установки с
     void sigSch(int dev);
public slots:
     //отображение измеряемых величин
     void refr();
     //слот реагирующий на сигнал с данными от класса MainWindow
     void  statuses(QVector<int> v,int alarm_code,QVector<int> r);
private slots:
     void set_dev_num(int dev_number);
     void send_signal();
private:
    Ui::widget_scheme *ui;
};

#endif // WIDGET_SCHEME_H
