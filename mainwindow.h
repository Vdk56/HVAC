#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QFile>
#include <QTextStream>
#include<QDateTime>
#include <QModbusTcpClient>
#include<QVector>
#include <QUrl>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QMainWindow>
#include <QGroupBox>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include "add.h"
#include "archive.h"
#include "vent_unit.h"
#include "widget_chart.h"
#include "widget_scheme.h"
#include "autorization.h"
#include<QDebug>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    //Метод инициализации, считывает файл конфигурации и вызывает метод refresh столько раз,
    //сколько устройств прописано в файле config
    void init();
    void refresh(QString name,QString ip, QString port, QString addr,int dev_num);
    // этот метод вызывается при добавлении или удалении устройства
    void in_ref();
    //метод создаёт соединение, вызывается из метода refresh
    void new_connection_tcp(QString name,QString ip, QString port, QString addr,int dev_num);
    //этот метод вызывается при удалении устройств, содержит деструкторы ообъектов
    //относящихся к удаляемому устройству
    void del_vectors(int i);
    QModbusTcpClient *modbusdevice = nullptr;
    //переменная, используемая для обозначения количества устройств
    int device_number;
    //переменная, используемая для обозначения первого цикла программы, например для того,
    // что бы записать коды конфигурации устройства в файл конфигурации
    bool first_cycle = 1;
    QTimer *t,*tim,*ttt;
    //три структуры со значениями последних прочитанных данных регистров
    struct InpStat{
         bool SCo_WintSum, SCo_RemLoc, SCo_timer, SCo_lock, SCo_remStartStop, SCo_locStartStop;
    };
    struct coils{
         bool SCi_RemLoc, SCi_timer, SCi_Start, SCi_Stop, SCi_faultRes;
    };
    struct holdingReg{
         int SCi_ref_t, SCi_ref_h, SCi_ref_co2, SCi_ref_change_ws, SCi_ref_season, SCi_ref_behave, Sci_fan_res;
    };
    //переменные со значениями температур для класса widget_chart
    int t11,t22,t33,t44;
    //переменная для отображения номера устройства, для котороко открыто окно управления
    int shown_unit;
    //указатель
    QModbusReply *lastRequest;
    QModbusDataUnit *readUnit;

    //Векторы с указателями на объекты, которые формируют виджеты устройств в стартовом окне
    QVector<QGroupBox*> GrB;
    QVector<QPushButton*> Buttons_V;
    QVector<QPushButton*> del_butt_V;
    QVector<QLabel*> status_label_V,temper_lab_v,state_lab_v;

    //В этом векторе хранятся все объекты QModbusTcpClient для упрощения доступа к нужному устройству
    QVector<QModbusTcpClient*> ModBus_Dev_vector;
    //здесь хранятся указатели на объекты QFile для всех устройств
    QVector<QFile*> archives_v;
    QVector<QFile*> messages_v;
    //Для хранения всех измеряемых величин отдельного устройства был выбран вектор строк, все экземпляры
    //таких векторов хранятся в векторе arch_data_strings_v соответственно порядковому номеру устройства
    QVector<QString> *archive;
    QList<QVector<QString>> arch_data_strings_v;

    //Коды конфигурации всех устройств
    QVector<quint32> SCADA_code;
    QVector<quint32> SENS_code;
    QVector<quint32> CPU_code;
    //Последние считанные коды аварий и состояний всех устройств, а также различных значения регистров
    QVector<int> alarm_code_v;
    QVector<int> state_code_v;
    QVector<InpStat*> InpStat_v;
    QVector<coils*> coils_v;
    QVector<holdingReg*> holdingReg_v;
    QVector<int>  *unit_states_v;
    QVector<QVector<int>> states_of_units_v;
    QVector<int>  *unit_readings_v;
    QVector<QVector<int>> readings_of_units_v;



    InpStat *input_status;
    coils *cls;
    holdingReg *hold_reg;

private slots:
    //обработка кнопки добавления устройства
    void on_add_but_clicked();
    //этот слот вызывается по сигналу от таймера для чтения регистров всех устройств. В нём вызывается метод requests,
    //в который отправляются аргументы с указанием номера устройства к которому будет запрос и числовым обозначением типа регистров
    //к которым будет отправлятся этот запрос
     void timer();
     void requests(int dev_num, int req_num);
    //слот вызывается от таймера tim для записи данных в архив
     void write_archive_sl();
     //обработка кнопок удаления устройств и кнопки отмены удаления
     void on_del_butt_clicked();
     void on_canc_but_clicked();

public slots:
  // слот вызывается по сигналу из класса ADD при добавлении устройства
  void in_refr();
  //обработка ответов на запросы
  void responce0();
  void responce1();
  void responce2();
  void responce3();
  void responce4();
  //удаление всех объектов связанных с устройством
  void deleteDevice();
  //отправить сигнал для класса widget_scheme для прорисовки мнемосхемы
   void send(int dev);
   //отправить сигнал для класса widget_chart с данными для графика
   void chart_data_slot(int u);
   //слот обработки сигналов от кнопок из окна управления для записи регистров
   void write(int dev,int type, int value);
   //отправка сигнала для класса vent_unit со статусами вент установки
   void send_unitState(int dev);
   //слот для обработки авторизации
   void login(int stat);
signals:
 void unitState(bool oper,bool zl, bool locRem, bool fault,int faults);
 void chart_data(int t11,int t22,int t33, int t44);
 void statusesMW(QVector<int> stat,int alarm_code,QVector<int> readings);
private:
    Ui::MainWindow *ui;
    Vent_unit v;
    widget_chart *w_ch = new widget_chart;
    widget_scheme *w_sch= new widget_scheme;
    autorization aut;
    QWidget vid;
    QGroupBox *v_un;
    QGridLayout *gr_l;
    QHBoxLayout *hb_l;
    QLabel *temp_l;
    QLabel *temp_lb;
    QLabel *status_l;
    QLabel *status_lb;
    QLabel *state_l;
    QLabel *state_lb;
    QPushButton *open_b,*del_b;
    QFont f;
    QFile *conf;
    QFile *messages;
    QFile *dev_archive;

 ADD *add = new ADD;

};
#endif // MAINWINDOW_H
