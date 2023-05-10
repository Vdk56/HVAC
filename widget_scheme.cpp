#include "widget_scheme.h"
#include "ui_widget_scheme.h"
 int statuses_arr[14];
 int readings_arr[20];
 int dev=0;

widget_scheme::widget_scheme(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::widget_scheme)
{
    ui->setupUi(this);

    timer_cycle_paint_ev = new QTimer;
    timer_cycle_paint_ev->setInterval(200);
    connect(timer_cycle_paint_ev,SIGNAL(timeout()),this,SLOT(refr()));
    timer_cycle_paint_ev->start();

    timer_signal.setInterval(500);
    connect(&timer_signal,SIGNAL(timeout()),this,SLOT(send_signal()));
    timer_signal.start();

    ui->p_bar_fan_in->hide();
    ui->p_bar_fan_out->hide();
    ui->p_bar_Kal1->hide();
    ui->p_bar_Kal2->hide();
    ui->p_bar_util->hide();
    ui->p_bar_uvlazhnit->hide();
    ui->p_bar_ohlad->hide();
    ui->p_bar_bypass->hide();
}

widget_scheme::~widget_scheme()
{
    delete ui;
}

void widget_scheme::init_pics(int scada_code, int sens_code,int cpu_code)
{

     nasos_run_gif.setFileName("./img/nasos_run.gif");
     nasos_run_gif.start();

     tube.load(":/new/img/truba.bmp");

     tube = tube.scaled(100,90,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
     jaluzi_inp_start.load(":/new/img/Jaluzi_free_start.bmp");

     if((scada_code&16)==16){
        fun_input_gif.setFileName("./img/fan_input.gif");
        fan_inp.load(":/new/img/fan_inp.bmp");
        fan_input_remen.load(":/new/img/Fan_input_remen.png");
        fan_input_thermo.load(":/new/img/Fan_input_thermo.png");
        fan_inp = fan_inp.scaled(100,95,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
        fan_input_thermo = fan_input_thermo.scaled(100,90,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
        fan_input_remen = fan_input_remen.scaled(100,90,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);

        if((cpu_code&1048576)==1048576){
            fan_inp_vfd.load(":/new/img/Fan_input_vfd.png");
            fan_input_vfderr.load(":/new/img/Fan_input_vfderr.png");
            fan_inp_vfd = fan_inp_vfd.scaled(100,95,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
            fan_input_vfderr = fan_input_vfderr.scaled(100,95,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
        }
     }

     if((scada_code&32)==32){
         fun_output_gif.setFileName("./img/fan_out.gif");
        fan_outp.load(":/new/img/fan_out.bmp");
        fan_out_rem_err.load(":/new/img/fan_out_rem_err.png");
        fan_out_termoerr.load(":/new/img/Fan_output_thermo.png");

        if((cpu_code&2097152)==2097152){
             fan_out_vfd.load(":/new/img/Fan_input_vfd.png");
             fan_output_vfderr.load(":/new/img/Fan_input_vfderr.png");
             fan_out_vfd = fan_out_vfd.scaled(100,90,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
             fan_output_vfderr = fan_output_vfderr.scaled(100,90,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
        }

        fan_outp = fan_outp.scaled(100,90,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
        fan_out_rem_err = fan_out_rem_err.scaled(100,90,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
        fan_out_termoerr = fan_out_termoerr.scaled(100,90,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);

    }

    if((scada_code&4)==4){
        filter_in.load(":/new/img/filter_in.bmp");
        filter_in_al_start.load(":/new/img/filter_in_al_start.bmp");
        filter_in_al_stop.load(":/new/img/filter_in_al_stop.bmp");
        filter_in_ok_stop.load(":/new/img/filter_in_ok_stop.bmp");
        filter_in = filter_in.scaled(100,90,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
        filter_out_al = filter_out_al.scaled(100,90,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
        filter_out_al_stop = filter_out_al_stop.scaled(100,90,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
        filter_in_ok_stop = filter_in_ok_stop.scaled(100,90,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
     }

      if((scada_code&8)==8){
        filter_out.load(":/new/img/filter_out.bmp");
        filter_out_al.load(":/new/img/filter_out_al.bmp");
        filter_out_al_stop.load(":/new/img/filter_out_al_stop.bmp");
        filter_output_ok_stop.load(":/new/img/Filter_output_ok_stop.bmp");
        filter_out = filter_out.scaled(100,90,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
        filter_out_al = filter_out_al.scaled(100,90,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
        filter_out_al_stop = filter_out_al_stop.scaled(100,90,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
        filter_output_ok_stop = filter_output_ok_stop.scaled(100,90,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
      }

        frege_pump_stop.load(":/new/img/frege_pump_stop.gif");
    if((scada_code&2)==2){
        jaluzi_inp_start.load(":/new/img/Jaluzi_free_start.bmp");
        jalusi_input_stop.load(":/new/img/Jaluzi_free_stop.bmp");
        jaluzi_free_stop.load(":/new/img/Jaluzi_free_start.bmp");
        jaluzi_in_heat.load(":/new/img/jaluzi_in_heat.bmp");
        jaluzi_inp_start=jaluzi_inp_start.scaled(100,90,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
        jalusi_input_stop=jalusi_input_stop.scaled(100,90,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
    }
    if((scada_code&1)==1){
        jalusi_input_stop_gif.setFileName(":/new/img/jalusi_input_stop.gif");
         jaluzi_inp_sens_op.load(":/new/img/jaluzi_inp_sens_op.bmp");
        jaluzi_inp_sens_op_err.load(":/new/img/jaluzi_inp_sens_op_err.bmp");
        jaluzi_inp_sens_close.load(":/new/img/jaluzi_inp_sens_close.bmp");
        jaluzi_inp_sens_close_err.load(":/new/img/jaluzi_inp_sens_close_err.bmp");
        jaluzi_in_err.load(":/new/img/jaluzi in_err.bmp");
        jaluzi_inp_sens_op=jaluzi_inp_sens_op.scaled(100,90,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
        jaluzi_inp_sens_op_err=jaluzi_inp_sens_op_err.scaled(100,90,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
        jaluzi_inp_sens_close=jaluzi_inp_sens_close.scaled(100,90,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
        jaluzi_inp_sens_close_err=jaluzi_inp_sens_close_err.scaled(100,90,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);

    }

     if(((cpu_code&192)==192)&((scada_code&128)==128)){
        air_gif.setFileName(":/new/img/air.gif");
        plast_util_al.load(":/new/img/plast_util_al.png");
        plast_util_not_used.load(":/new/img/plast_util_not_used.bmp");
        plast_util_stop.load(":/new/img/plast_util_stop.bmp");
        plast_util_freeze.load(":/new/img/plast_util_freeze.png");
        if((cpu_code&768)==768){
            bypass_open.load(":/new/img/bypass_open.bmp");
            bypass_close.load(":/new/img/bypass_close.bmp");
            bypass_open= bypass_open.scaled(90,59,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
            bypass_close= bypass_close.scaled(90,59,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
        }
        plast_util_stop = plast_util_stop.scaled(200,279,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
        plast_util_al = plast_util_al.scaled(200,279,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
        plast_util_freeze = plast_util_freeze.scaled(200,279,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);

     }
     if(((cpu_code&192)==192)&((scada_code&128)==0)){
         rotor_ut_start_gif.setFileName(":/new/img/rotor_ut_start.gif");
         rotor_util.load(":/new/img/rotor_util.bmp");
         rotor_util_alarm.load(":/new/img/rotor_util_alarm.png");
         rotor_util_freeze.load(":/new/img/rotor_util_freeze.png");
         rotor_util = rotor_util.scaled(200,218,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
         rotor_util_alarm = rotor_util_alarm.scaled(200,218,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
         rotor_util_freeze = rotor_util_freeze.scaled(200,218,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);

     }

     if((((CPU_CODE&4)==4)or((CPU_CODE&8)==8))&((SCADA_CODE&2048)==2048)){
         coller_nasos_runKal2.setFileName(":/new/img/coller_nasos_run.gif");
            vod_kal_start_gif.setFileName("./img/fan_input.gif");
            Vod_Kal_alarm.load(":/new/img/Vod_Kal_alarm.png");
            Vod_Kal_start1.load(":/new/img/Vod_Kal_start1.png");
            Vod_Kal_start2.load(":/new/img/Vod_Kal_start2.png");
            Vod_Kal_start3.load(":/new/img/Vod_Kal_start3.png");
            Vod_Kal_stop.load(":/new/img/Vod_Kal_stop.png");
            Vod_Kal_alarm= Vod_Kal_alarm.scaled(100,200,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
            Vod_Kal_stop= Vod_Kal_stop.scaled(100,200,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
            Vod_Kal_start1= Vod_Kal_start1.scaled(100,200,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
            Vod_Kal_start2= Vod_Kal_start2.scaled(100,200,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
            Vod_Kal_start3= Vod_Kal_start3.scaled(100,200,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
     }

     if(((CPU_CODE&12)==12)&((SCADA_CODE&2048)==0)){
            TEN_cooled_gif.setFileName(":/new/img/TEN_cooled.gif");
            TEN_alarm.load(":/new/img/TEN_alarm.bmp");
            TEN_cooled.load(":/new/img/TEN_cooled.gif");
            TEN_not_used.load(":/new/img/TEN_not_used.bmp");
            TEN_start1.load(":/new/img/TEN_start1.png");
            TEN_start2.load(":/new/img/TEN_start2.png");
            TEN_start3.load(":/new/img/TEN_start3.png");
            Ten1_dis.load(":/new/img/Jaluzi_free_start.bmp");
            TEN_stoped.load(":/new/img/TEN_stop.bmp");
            TEN_stoped=TEN_stoped.scaled(100,95,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
            TEN_alarm=TEN_alarm.scaled(100,95,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
            TEN_start1=TEN_start1.scaled(100,95,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
            TEN_start2=TEN_start2.scaled(100,95,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
            TEN_start3=TEN_start3.scaled(100,95,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
    }

    Winter.load(":/new/img/Winter.bmp");
    Summer.load(":/new/img/Summer.bmp");



    if(((CPU_CODE&2)==2)or((CPU_CODE&1)==1)){
         coller_nasos_runKal1.setFileName(":/new/img/coller_nasos_run.gif");
            Vod_Kal_alarm.load(":/new/img/Vod_Kal_alarm.png");
            Vod_Kal_start1.load(":/new/img/Vod_Kal_start1.png");
            Vod_Kal_start2.load(":/new/img/Vod_Kal_start2.png");
            Vod_Kal_start3.load(":/new/img/Vod_Kal_start3.png");
            Vod_Kal_stop.load(":/new/img/Vod_Kal_stop.png");
            Vod_Kal_alarm= Vod_Kal_alarm.scaled(100,200,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
            Vod_Kal_stop= Vod_Kal_stop.scaled(100,200,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
            Vod_Kal_start1= Vod_Kal_start1.scaled(100,200,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
            Vod_Kal_start2= Vod_Kal_start2.scaled(100,200,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
            Vod_Kal_start3= Vod_Kal_start3.scaled(100,200,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
    }

    if((cpu_code&3072)==3072){
            if((scada_code&4096)==4096){
                oros_h_start_gif.setFileName(":/new/img/oros_hum_start.gif");
                oros_uvlaz_alarm.load(":/new/img/oros_uvlaz_alarm.png");
                oros_uvlaz_not_used.load(":/new/img/oros_uvlaz_not_used.bmp");
                oros_uvlaz_stop.load(":/new/img/oros_uvlaz_stop.bmp");
                oros_uvlaz_alarm=oros_uvlaz_alarm.scaled(100,95,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
                oros_uvlaz_stop=oros_uvlaz_stop.scaled(100,95,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
            }

            if((scada_code&4096)==0){
                steam_h_start_gif.setFileName(":/new/img/steam_hum_start.gif");
                steam_hum_alarm.load(":/new/img/steam_hum_alarm.png");
                steam_hum_stop.load(":/new/img/steam_hum_stop.bmp");
                steam_hum_alarm=steam_hum_alarm.scaled(100,200,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
                steam_hum_stop=steam_hum_stop.scaled(100,200,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);

            }
    }

    if((scada_code&262144)==262144){
        recyrc_util_alarm.load(":/new/img/recyrc_util_alarm.bmp");
        recyrc_util_not_used.load(":/new/img/recyrc_util_not_used.bmp");
        recyrc_util_start.load(":/new/img/recyrc_util_start.bmp");
        recyrc_util_stop.load(":/new/img/recyrc_util_stop.bmp");
        recyrc_util_start=recyrc_util_start.scaled(70,80,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
        recyrc_util_alarm=recyrc_util_alarm.scaled(70,80,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
        recyrc_util_stop=recyrc_util_stop.scaled(70,80,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
        recyrc_util_not_used=recyrc_util_not_used.scaled(70,80,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
    }

    if(((cpu_code&48)==48) && ((scada_code&16384)==0)){
        coller_nasos_run.setFileName(":/new/img/coller_nasos_run.gif");
        coller_pump_al.load(":/new/img/Cooler_nasos_alarm.png");
        coller_not_used.load(":/new/img/Cooler_not_used.bmp");
        coller_not_used_pump.load(":/new/img/Cooler_not_used_nasos.bmp");
        coller_start_1.load(":/new/img/Cooler_start1.png");
        coller_start_2.load(":/new/img/Cooler_start2.png");
        coller_start_3.load(":/new/img/Cooler_start3.png");
        coller_stop.load(":/new/img/Cooler_stop.bmp");
        coller_stop_pump.load(":/new/img/Cooler_stop_nasos.bmp");
        coller_stop= coller_stop.scaled(100,200,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
        coller_pump_al = coller_pump_al.scaled(100,50,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
    }

    if(((cpu_code&48)==48) && ((scada_code&16384)==16384)){
         frege_pump_stop_gif.setFileName("./img/frege_pump_stop.gif");
        freon_oxl_alarm.load(":/new/img/freon_oxl_alarm.png");
        freon_oxl_freeze.load(":/new/img/freon_oxl_freeze.png");
        freon_oxl_not_used.load(":/new/img/freon_oxl_not_used.bmp");
        freon_oxl_pusk.load(":/new/img/freon_oxl_pusk.png");
        freon_oxl_stop.load(":/new/img/freon_oxl_stop.bmp");
        freon_oxl_freeze=freon_oxl_freeze.scaled(100,95,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
        freon_oxl_stop=freon_oxl_stop.scaled(100,95,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
        freon_oxl_alarm=freon_oxl_alarm.scaled(34,52,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
        freon_oxl_pusk=freon_oxl_pusk.scaled(100,95,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
    }

    wall_outside.load(":/new/img/Wall_street1.bmp");
    wall_inside.load(":/new/img/Wall_room1.bmp");
    wall_outside=wall_outside.scaled(100,400,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
}



void widget_scheme::refr()
{
    ui->p_bar_util->setValue(readings_arr[14]);
    ui->p_bar_bypass->setValue(readings_arr[15]);
    ui->p_bar_ohlad->setValue(readings_arr[16]);
    ui->p_bar_uvlazhnit->setValue(readings_arr[17]);
    ui->p_bar_fan_in->setValue(readings_arr[18]);
    ui->p_bar_fan_out->setValue(readings_arr[19]);
    ui->p_bar_Kal1->setValue(statuses_arr[11]);
    ui->p_bar_Kal2->setValue(statuses_arr[12]);
    update();
    repaint();
}

void widget_scheme::set_dev_num(int dev_number)
{
    dev=dev_number-1;
    f = new QFile("config.cfg");
    f->open(QIODevice::ReadOnly);
    QString content;
    for(;!f->atEnd();) content = content + f->readLine();
    QJsonDocument jdoc = QJsonDocument::fromJson(content.toUtf8());
    QJsonObject dev = jdoc.object();
    QJsonArray arr_dev = dev["Devices"].toArray();
    QJsonObject sub_dev_obj =  arr_dev[dev_number-1].toObject();
    SCADA_CODE = sub_dev_obj["Device_config"].toString().toInt();
    SENS_CODE = sub_dev_obj["Sensors_config"].toString().toInt();
    CPU_CODE = sub_dev_obj["CPU_config"].toString().toInt();
    init_pics(SCADA_CODE, SENS_CODE,CPU_CODE);
}

void widget_scheme::statuses(QVector<int> v,int alarm_code, QVector<int> r)
{
    ALARM_CODE=alarm_code;
    for (int i = 0;i<14;i++) {
        statuses_arr[i]=v[i];
    }
    for (int i = 0;i<20;i++) {
        readings_arr[i]=r[i];
    }

}

void widget_scheme::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);     
    painter.setViewport(0, 0, 825, 350);

    ui->lab_t_ulitsa->setText("Снаружи "+QString::number(readings_arr[0])+"°C");
    ui->lab_t_kanal->setText("Канал "+QString::number(readings_arr[1])+"°C");
    ui->lab_t_obr->setText("t°C обр. "+QString::number(readings_arr[2])+"°C");
    ui->lab_t_pomesh->setText("Помещение "+QString::number(readings_arr[3])+"°C");
    ui->lab_t_vitayzhka->setText("Вытяжка "+QString::number(readings_arr[4])+"°C");
    ui->lab_h_pomesh->setText("Влажность "+QString::number(readings_arr[5])+"%");
    ui->lab_t_ustavka->setText("Уставка t°C "+QString::number(readings_arr[11])+"°C");
    ui->lab_h_ustavka->setText("Уставка h "+QString::number(readings_arr[12])+"%");


    painter.drawImage(10,0,wall_outside);
    painter.drawImage(1030,7,wall_inside);

    painter.drawImage(230,100,tube);
    painter.drawImage(330,100,tube);
    painter.drawImage(430,100,tube);
    painter.drawImage(530,100,tube);
    painter.drawImage(630,100,tube);
    painter.drawImage(730,100,tube);
    painter.drawImage(830,100,tube);
    painter.drawImage(930,100,tube);
    painter.drawImage(230,228,tube);
    painter.drawImage(330,228,tube);
    painter.drawImage(430,228,tube);
    painter.drawImage(530,228,tube);
    painter.drawImage(630,228,tube);
    painter.drawImage(730,228,tube);
    painter.drawImage(230,228,tube);


 if((SCADA_CODE&2)==2){
     painter.drawImage(61,100,jalusi_input_stop);

        if ((statuses_arr[1]&1)==1){
            painter.drawImage(61,100,jaluzi_inp_start);
         }
}
if((SCADA_CODE&1)==1){
    painter.drawImage(61,228,jaluzi_inp_sens_close); //стоп приток
    if ((statuses_arr[0]&1)==1){
            painter.drawImage(61,228,jaluzi_inp_sens_op);//работа приток
            if ((statuses_arr[0]&4)==4){
            painter.drawPixmap(61,228,jalusi_input_stop_gif.currentPixmap());
             jalusi_input_stop_gif.start();
            } else     jalusi_input_stop_gif.stop();
    }
    if ((statuses_arr[0]&3)==3){
         painter.drawImage(61,228,jaluzi_inp_sens_op_err);//работа ошибка приток
    }
        if ((statuses_arr[0]&4)==4){
        painter.drawPixmap(61,228,jalusi_input_stop_gif.currentPixmap());
         jalusi_input_stop_gif.start();
        } else     jalusi_input_stop_gif.stop();
    if ((statuses_arr[0]&2)==2){
         painter.drawImage(61,228,jaluzi_inp_sens_close_err);//стоп ошибка приток
     }
}

if((SCADA_CODE&32)==32){
    painter.drawImage(140,100,fan_outp);
     painter.drawImage(630,100,filter_output_ok_stop);
     if ((statuses_arr[3]&1)==1){
          painter.drawPixmap(140,100,fun_output_gif.currentPixmap());
          fun_output_gif.start();
          painter.drawImage(630,100,filter_out);
          if((ALARM_CODE&131072)==131072){
                painter.drawImage(630,100,filter_out_al);
          }

    }else fun_output_gif.stop();

     if((ALARM_CODE&131072)==131072){
            painter.drawImage(630,100,filter_out_al_stop);
      }

    if ((statuses_arr[3]&2)==2){
          painter.drawImage(140,100,fan_out_rem_err);
    }
    if ((statuses_arr[3]&256)==256){
          painter.drawImage(140,100,fan_out_termoerr);
    }
    if((CPU_CODE&2097152)==2097152){
          painter.drawImage(140,100,fan_out_vfd);
          ui->p_bar_fan_out->show();
          if ((statuses_arr[3]&1024)==1024){
                painter.drawImage(140,100,fan_output_vfderr);
          }
   }
}

if((SCADA_CODE&16)==16){
       painter.drawImage(930,228,fan_inp);
       painter.drawImage(150,228,filter_in_ok_stop);
       if ((statuses_arr[2]&1)==1){
                painter.drawPixmap(930,228,fun_input_gif.currentPixmap());
                fun_input_gif.start();
                painter.drawImage(150,228,filter_in);
                if((ALARM_CODE&32768)==32768){
                        painter.drawImage(150,228,filter_in_al_start);
                 }

       }else fun_input_gif.stop();

       if((ALARM_CODE&32768)==32768){
                   painter.drawImage(150,228,filter_in_al_stop);
       }

       if ((statuses_arr[2]&2)==2){
                painter.drawImage(930,228,fan_input_remen);
       }

        if ((statuses_arr[2]&256)==256){
                painter.drawImage(930,228,fan_input_thermo);
        }

        if((CPU_CODE&1048576)==1048576){
               ui->p_bar_fan_in->show();
                painter.drawImage(930,228,fan_inp_vfd);
                if ((statuses_arr[2]&1024)==1024){
                        painter.drawImage(930,228,fan_input_vfderr);
                }
        }
}

if(((CPU_CODE&48)==48) && ((SCADA_CODE&16384)==0)){
  painter.drawImage(530,228,coller_stop);
  ui->p_bar_ohlad->show();
  if ((statuses_arr[10]&1)==1){
//        painter.drawImage(530,228,coller_start_1);
//        painter.drawImage(530,228,coller_start_2);
        painter.drawImage(530,228,coller_start_3);
  }
  if ((statuses_arr[10]&1)==1){
  painter.drawImage(530,378,coller_pump_al);
  }
  if ((statuses_arr[10]&128)==128){
      painter.drawImage(730,270,coller_pump_al);
  }
  if ((statuses_arr[10]&64)==64){
      painter.drawPixmap(530,270,coller_nasos_run.currentPixmap());
      coller_nasos_run.start();
  }else coller_nasos_run.stop();
}

if(((CPU_CODE&48)==48) && ((SCADA_CODE&16384)==16384)){
  ui->p_bar_ohlad->show();
  painter.drawImage(530,228,freon_oxl_stop);
  if ((statuses_arr[10]&1)==1){
  painter.drawImage(530,228,freon_oxl_pusk);
  }
  if ((statuses_arr[10]&512)==512){
  painter.drawImage(530,228,freon_oxl_freeze);
  }
  if ((statuses_arr[10]&2)==2){
  painter.drawImage(530,272,freon_oxl_alarm);
  }
}

if(((CPU_CODE&192)==192)&((SCADA_CODE&128)==128)){
    ui->p_bar_util->show();

        painter.drawImage(330,40,plast_util_stop);

    if ((statuses_arr[7]&2)==2){
            painter.drawImage(330,40,plast_util_al);
     }
    if ((statuses_arr[7]&512)==512){
            painter.drawImage(330,40,plast_util_freeze);
    }
    if ((statuses_arr[7]&1)==1){
            painter.drawPixmap(330,40,air_gif.currentPixmap());
            air_gif.start();
     }else air_gif.stop();
    if((CPU_CODE&768)==768){
            painter.drawImage(397,39,bypass_close);
             ui->p_bar_bypass->show();
            if ((statuses_arr[8]&1)==1){
                painter.drawImage(397,39,bypass_open);
            }
    }
}

if(((CPU_CODE&192)==192)&((SCADA_CODE&128)==0)){
      ui->p_bar_util->show();
        painter.drawImage(330,100,rotor_util);
        if ((statuses_arr[7]&2)==2){
            painter.drawImage(330,100,rotor_util_alarm);
        }
        if ((statuses_arr[7]&512)==512){
        painter.drawImage(330,100,rotor_util_freeze);
        }
        if ((statuses_arr[7]&1)==1){
            painter.drawPixmap(375,170,rotor_ut_start_gif.currentPixmap());
            rotor_ut_start_gif.start();
        }else rotor_ut_start_gif.stop();
}

 if((SCADA_CODE&262144)==262144){
     painter.drawImage(240,160,recyrc_util_stop);
    if ((statuses_arr[8]&1)==1){
    painter.drawImage(240,160,recyrc_util_start);
    }
    if ((statuses_arr[8]&2)==2){
    painter.drawImage(240,160,recyrc_util_alarm);
    }
}

if(((CPU_CODE&12)==12)&((SCADA_CODE&2048)==0)){
        painter.drawImage(730,228,TEN_stoped);
        if ((statuses_arr[6]&2)==2){
            painter.drawImage(730,228,TEN_alarm);
        }
        if ((statuses_arr[6]&1)==1){
             if ((statuses_arr[13]&1)==1){
                    painter.drawImage(730,228,TEN_start1);
              }
             if ((statuses_arr[13]&3)==3){
                    painter.drawImage(730,228,TEN_start2);
              }
              if ((statuses_arr[13]&7)==7){
                    painter.drawImage(730,228,TEN_start3);
              }
        }
        if ((statuses_arr[6]&5)==5){
              painter.drawPixmap(738,231,TEN_cooled_gif.currentPixmap());
              TEN_cooled_gif.start();
        }else TEN_cooled_gif.stop();
}

if(((CPU_CODE&2)==2)or((CPU_CODE&1)==1)){
    ui->p_bar_Kal1->show();
    painter.drawImage(630,228,Vod_Kal_stop);
    if ((statuses_arr[4]&1)==1){

            painter.drawImage(630,228,Vod_Kal_start3);

         if ((statuses_arr[4]&64)==64){
            painter.drawImage(630,228,Vod_Kal_start2);
          }
         if ((statuses_arr[4]&128)==128){
            painter.drawImage(630,228,Vod_Kal_start3);
         }
    }
    if ((statuses_arr[4]&2)==2){
        painter.drawImage(630,228,Vod_Kal_alarm);
   }
    if ((statuses_arr[5]&2)==2){
        painter.drawImage(630,270,coller_pump_al);
    }
    if ((statuses_arr[5]&1)==1){
        painter.drawPixmap(637,400,coller_nasos_runKal1.currentPixmap());
        coller_nasos_runKal1.start();
    }else coller_nasos_runKal1.stop();
}

if((((CPU_CODE&4)==4)or((CPU_CODE&8)==8))&((SCADA_CODE&2048)==2048)){
       ui->p_bar_Kal2->show();
        painter.drawImage(730,228,Vod_Kal_stop);
        if ((statuses_arr[6]&2)==2){
                painter.drawImage(730,228,Vod_Kal_alarm);
        }
        if ((statuses_arr[6]&1)==1){

                    painter.drawImage(730,228,Vod_Kal_start1);

             if ((statuses_arr[6]&64)==64){
                    painter.drawImage(730,228,Vod_Kal_start2);
             }
             if ((statuses_arr[6]&128)==128){
                    painter.drawImage(730,228,Vod_Kal_start3);
             }
        }
        if ((statuses_arr[5]&16)==16){
            painter.drawImage(735,270,coller_pump_al);
        }
        if ((statuses_arr[5]&8)==8){
            painter.drawPixmap(737,400,coller_nasos_runKal2.currentPixmap());
            coller_nasos_runKal2.start();
        }else coller_nasos_runKal2.stop();
}

if((CPU_CODE&3072)==3072){
    if((SCADA_CODE&4096)==0){
        ui->p_bar_uvlazhnit->show();
        painter.drawImage(830,228,steam_hum_stop);
       if ((statuses_arr[9]&1)==1){
           painter.drawPixmap(865,192,steam_h_start_gif.currentPixmap());
           painter.drawPixmap(865,205,steam_h_start_gif.currentPixmap());
           painter.drawPixmap(865,218,steam_h_start_gif.currentPixmap());
           steam_h_start_gif.start();
       }else   steam_h_start_gif.stop();
       if ((statuses_arr[9]&2)==2){
        painter.drawImage(830,228,steam_hum_alarm);
       }
    }

    if((SCADA_CODE&4096)==4096){
        ui->p_bar_uvlazhnit->show();
        painter.drawImage(830,228,oros_uvlaz_stop);
        if ((statuses_arr[9]&1)==1){
            painter.drawPixmap(835,248,oros_h_start_gif.currentPixmap());
            oros_h_start_gif.start();
        }
        if ((statuses_arr[9]&2)==2){
            painter.drawImage(830,228,oros_uvlaz_alarm);
        }
   }
}
    painter.save();
    painter.restore();
}

void widget_scheme::send_signal()
{
 emit sigSch(dev);
}


