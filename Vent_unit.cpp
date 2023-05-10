#include "vent_unit.h"
#include "ui_vent_unit.h"
bool f;
int faults_prev=0;
Vent_unit::Vent_unit(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Vent_unit)
{
    ui->setupUi(this);
    timer_cycle = new QTimer;
    timer_cycle->setInterval(500);
    connect(timer_cycle, SIGNAL(timeout()), this, SLOT(time()));
    ws = new widget_scheme(ui->widget);
    wch = new  widget_chart(ui->widget_chart);
    connect(this ,SIGNAL(unit_number(int)),wch,SLOT(set_dev(int)));
    connect(this ,SIGNAL(unit_number(int)),ws,SLOT(set_dev_num(int)));
    ui->messages->setColumnWidth(0,200);
    ui->messages->setColumnWidth(1,535);

}

Vent_unit::~Vent_unit()
{
    delete ui;
}

void Vent_unit::fill_table_init(int unit)
{
    QString path ="messages_"+QString::number(unit)+".txt";
    QFile file(path);
    file.open(QIODevice::ReadOnly);

    QString line;
    QTextStream stream(&file);
    QStringList list;
    do{
        line= stream.readLine();

        if(!line.isNull()){
            list = line.split(";");
            ui->messages->insertRow(0);
            ui->messages->setItem(0, 0,new QTableWidgetItem(list[0]));
            ui->messages->setItem(0, 1, new QTableWidgetItem(list[1]));

        }

    }while(!line.isNull());
     file.close();
}

void Vent_unit::show_unit()
{
    QStringList obj_name = sender()->objectName().split("_");
    int i = obj_name[1].toInt();
    ch_unit = i;

    emit unit_number(ch_unit);
     timer_cycle->start();
      ui->ust_T_lab->setText("15 °C");
      ui->ust_H_lab->setText("0 %");
      ui->ust_ZL_lab->setText("10 °C");
     zim.load(":/new/img/Winter.bmp");
     let.load(":/new/img/Summer.bmp");
     for (int i=0;i<31;i++) {
         items_v.push_back(new items_str);
     }

     fill_table_init(ch_unit);
    this->show();
    ui->ustavka_t->setRange(15,30);
    ui->ustavka_h->setRange(0,100);
    ui->ustZL->setRange(-10,30);
    ui->ustavka_t->setTickInterval(1);
    ui->ustavka_t->setTickPosition(QSlider::TicksBelow);
    ui->ustavka_h->setTickInterval(10);
    ui->ustavka_h->setTickPosition(QSlider::TicksBelow);
}

void Vent_unit::time()
{
    emit unitS(ch_unit);
}

void Vent_unit::unitStat(bool oper, bool zl, bool locRem, bool fault, int faults)
{
       f=fault;
       if(oper) ui->stat_lab->setStyleSheet(
                    "background-color: rgb(0, 255, 0);"
                    "border: 2px solid gray;"
                    "border-radius: 5px;"
                    "margin-top: 1ex;"
                   );
       else  ui->stat_lab->setStyleSheet(
                   "background-color: rgb(230,230, 230);"
                   "border: 2px solid gray;"
                   "border-radius: 5px;"
                   "margin-top: 1ex;"
                   );
       if(fault) ui->stat_lab->setStyleSheet(
                   "background-color: rgb(255, 0, 0);"
                   "border: 2px solid gray;"
                   "border-radius: 5px;"
                   "margin-top: 1ex;"
                  );
       if(locRem) ui->remLoc_butt->setText("Дист.");
       else ui->remLoc_butt->setText("Мест.");
       if(zl) ui->ZL_lab->setPixmap(zim.scaled(50,50,Qt::IgnoreAspectRatio));
       else ui->ZL_lab->setPixmap(let.scaled(50,50,Qt::IgnoreAspectRatio));

       if(faults_prev!=faults) messages_journ(faults, faults_prev);
       faults_prev=faults;
}

void Vent_unit::on_ustavka_t_valueChanged(int value)
{
    ui->ust_T_lab->setText(QString::number(value)+"°C");
    emit em(ch_unit,0,value);
}

void Vent_unit::on_ustavka_h_valueChanged(int value)
{
     ui->ust_H_lab->setText(QString::number(value)+"%");
    emit em(ch_unit,1,value);
}

void Vent_unit::on_ustZL_valueChanged(int value)
{
    ui->ust_ZL_lab->setText(QString::number(value)+"°C");
    emit em(ch_unit,2,value);
}

void Vent_unit::on_pusk_butt_clicked()
{
    emit em(ch_unit,3,1);
}

void Vent_unit::on_stop_butt_clicked()
{
emit em(ch_unit,4,1);
}

void Vent_unit::on_remLoc_butt_clicked()
{
    int i;
    if(ui->remLoc_butt->text()=="Дист.") i=0;
    if(ui->remLoc_butt->text()=="Мест.") i=1;
    emit em(ch_unit,5,i);
}

void Vent_unit::on_reset_butt_clicked()
{
    emit em(ch_unit,6,1);
}

void Vent_unit::messages_journ(int faults,int previous)
{
    QString message="";
    QColor red(255,0,0,255);
    QColor white(255,255,255,255);
    if(((previous&1)==0)&((faults&1)==1)){
        ui->messages->insertRow(0);
        items_str *i = new items_str;
        i->item1->setText(QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss"));
        i->item2->setText("Ошибка насоса калорифера 2");
        items_v[0]=i;
        ui->messages->setItem(0, 0,i->item1);
        ui->messages->setItem(0, 1, i->item2);
        i->item1->setBackground(red);
       i->item2->setBackground(red);
        message = QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+";";
        message.append("Ошибка насоса калорифера 2");
        arh->write_fault_message(message,ch_unit);
    }
     if(((previous&1)==1)&((faults&1)==0)){
       items_v[0]->item1->setBackground(white);
       items_v[0]->item2->setBackground(white);
     }

    if(((previous&2)==0)&((faults&2)==2)){
        ui->messages->insertRow(0);
        items_str *i = new items_str;
        i->item1->setText(QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss"));
        i->item2->setText("Ошибка жалюзей притока");
        items_v[1]=i;
        ui->messages->setItem(0, 0,i->item1);
        ui->messages->setItem(0, 1, i->item2);
        i->item1->setBackground(red);
       i->item2->setBackground(red);
        ui->messages->setItem(0, 0,  i->item1);
        ui->messages->setItem(0, 1, i->item2);
        message = QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+";";
        message.append("Ошибка жалюзей притока");
        arh->write_fault_message(message,ch_unit);
    }
     if(((previous&2)==2)&((faults&2)==0)){
         items_v[1]->item1->setBackground(white);
         items_v[1]->item2->setBackground(white);
     }

    if(((previous&4)==0)&((faults&4)==4)){
        ui->messages->insertRow(0);
        items_str *i = new items_str;
        i->item1->setText(QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss"));
        i->item2->setText("Ошибка жалюзей вытяжки");
        items_v[2]=i;
        ui->messages->setItem(0, 0,i->item1);
        ui->messages->setItem(0, 1, i->item2);
        i->item1->setBackground(red);
       i->item2->setBackground(red);
       ui->messages->setItem(0, 0,  i->item1);
       ui->messages->setItem(0, 1, i->item2);
        message = QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+";";
        message.append("Ошибка жалюзей вытяжки");
        arh->write_fault_message(message,ch_unit);
    }
     if(((previous&4)==4)&((faults&4)==0)){
         items_v[2]->item1->setBackground(white);
         items_v[2]->item2->setBackground(white);
     }

    if(((previous&8)==0)&((faults&8)==8)){
        ui->messages->insertRow(0);
        items_str *i = new items_str;
        i->item1->setText(QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss"));
        i->item2->setText("Ошибка вентилятора притока (общая)");
        items_v[3]=i;
        ui->messages->setItem(0, 0,i->item1);
        ui->messages->setItem(0, 1, i->item2);
        i->item1->setBackground(red);
       i->item2->setBackground(red);
       ui->messages->setItem(0, 0,  i->item1);
       ui->messages->setItem(0, 1, i->item2);
        message = QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+";";
        message.append("Ошибка вентилятора притока (общая)");
        arh->write_fault_message(message,ch_unit);
    }
    if(((previous&8)==8)&((faults&8)==0)){
        items_v[3]->item1->setBackground(white);
        items_v[3]->item2->setBackground(white);
    }

    if(((previous&16)==0)&((faults&16)==16)){
        ui->messages->insertRow(0);
        items_str *i = new items_str;
        i->item1->setText(QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss"));
        i->item2->setText("Ошибка вентилятора вытяжки (общая)");
        items_v[4]=i;
        ui->messages->setItem(0, 0,i->item1);
        ui->messages->setItem(0, 1, i->item2);
        i->item1->setBackground(red);
       i->item2->setBackground(red);
       ui->messages->setItem(0, 0,  i->item1);
       ui->messages->setItem(0, 1, i->item2);
        message = QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+";";
        message.append("Ошибка вентилятора вытяжки (общая)");
        arh->write_fault_message(message,ch_unit);
    }
    if(((previous&16)==16)&((faults&16)==0)){
        items_v[4]->item1->setBackground(white);
        items_v[4]->item2->setBackground(white);
    }

    if(((previous&32)==0)&((faults&32)==32)){
        ui->messages->insertRow(0);
        items_str *i = new items_str;
        i->item1->setText(QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss"));
        i->item2->setText("Заморозка");
        items_v[5]=i;
        ui->messages->setItem(0, 0,i->item1);
        ui->messages->setItem(0, 1, i->item2);
        i->item1->setBackground(red);
       i->item2->setBackground(red);
       ui->messages->setItem(0, 0,  i->item1);
       ui->messages->setItem(0, 1, i->item2);
        message = QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+";";
        message.append("Заморозка");
        arh->write_fault_message(message,ch_unit);
    }
     if(((previous&32)==32)&((faults&32)==0)){

         items_v[5]->item1->setBackground(white);
         items_v[5]->item2->setBackground(white);
     }

    if(((previous&64)==0)&((faults&64)==64)){
        ui->messages->insertRow(0);
        items_str *i = new items_str;
        i->item1->setText(QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss"));
        i->item2->setText("Ошибка насоса калорифера 1");
        items_v[6]=i;
        ui->messages->setItem(0, 0,i->item1);
        ui->messages->setItem(0, 1, i->item2);
        i->item1->setBackground(red);
       i->item2->setBackground(red);
       ui->messages->setItem(0, 0,  i->item1);
       ui->messages->setItem(0, 1, i->item2);
        message = QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+";";
        message.append("Ошибка насоса калорифера 1");
        arh->write_fault_message(message,ch_unit);
    }
     if(((previous&64)==64)&((faults&64)==0)){
         items_v[6]->item1->setBackground(white);
         items_v[6]->item2->setBackground(white);
     }

    if(((previous&128)==0)&((faults&128)==128)){
        ui->messages->insertRow(0);
        items_str *i = new items_str;
        i->item1->setText(QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss"));
        i->item2->setText("Ошибка электрокалорифера");
        items_v[7]=i;
        ui->messages->setItem(0, 0,i->item1);
        ui->messages->setItem(0, 1, i->item2);
        i->item1->setBackground(red);
       i->item2->setBackground(red);
       ui->messages->setItem(0, 0,  i->item1);
       ui->messages->setItem(0, 1, i->item2);
        message = QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+";";
        message.append("Ошибка электрокалорифера");
        arh->write_fault_message(message,ch_unit);
    }
     if(((previous&128)==128)&((faults&128)==0)){
         items_v[7]->item1->setBackground(white);
         items_v[7]->item2->setBackground(white);
     }

    if(((previous&256)==0)&((faults&256)==256)){
        ui->messages->insertRow(0);
        items_str *i = new items_str;
        i->item1->setText(QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss"));
        i->item2->setText("Ошибка компрессора");
        items_v[8]=i;
        ui->messages->setItem(0, 0,i->item1);
        ui->messages->setItem(0, 1, i->item2);
        i->item1->setBackground(red);
       i->item2->setBackground(red);
       ui->messages->setItem(0, 0,  i->item1);
       ui->messages->setItem(0, 1, i->item2);
        message = QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+";";
        message.append("Ошибка компрессор");
        arh->write_fault_message(message,ch_unit);
    }
     if(((previous&256)==256)&((faults&256)==0)){
         items_v[8]->item1->setBackground(white);
         items_v[8]->item2->setBackground(white);
     }

    if(((previous&512)==0)&((faults&512)==512)){
        ui->messages->insertRow(0);
        items_str *i = new items_str;
        i->item1->setText(QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss"));
        i->item2->setText("Ошибка рекуператора");
        items_v[9]=i;
        ui->messages->setItem(0, 0,i->item1);
        ui->messages->setItem(0, 1, i->item2);
        i->item1->setBackground(red);
       i->item2->setBackground(red);
       ui->messages->setItem(0, 0,  i->item1);
       ui->messages->setItem(0, 1, i->item2);
        message = QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+";";
        message.append("Ошибка рекуператора");
        arh->write_fault_message(message,ch_unit);
    }
    if(((previous&512)==512)&((faults&512)==0)){
        items_v[9]->item1->setBackground(white);
        items_v[9]->item2->setBackground(white);
    }

    if(((previous&1024)==0)&((faults&1024)==1024)){
        ui->messages->insertRow(0);
        items_str *i = new items_str;
        i->item1->setText(QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss"));
        i->item2->setText("Ошибка увлажнителя");
        items_v[10]=i;
        ui->messages->setItem(0, 0,i->item1);
        ui->messages->setItem(0, 1, i->item2);
        i->item1->setBackground(red);
       i->item2->setBackground(red);
       ui->messages->setItem(0, 0,  i->item1);
       ui->messages->setItem(0, 1, i->item2);
        message = QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+";";
        message.append("Ошибка увлажнителя");
        arh->write_fault_message(message,ch_unit);
    }
    if(((previous&1024)==1024)&((faults&1024)==0)){
        items_v[10]->item1->setBackground(white);
        items_v[10]->item2->setBackground(white);
   }


    if(((previous&2048)==0)&((faults&2048)==2048)){
        ui->messages->insertRow(0);
        items_str *i = new items_str;
        i->item1->setText(QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss"));
        i->item2->setText("Ошибка  насоса вод.охладителя");
        items_v[11]=i;
        ui->messages->setItem(0, 0,i->item1);
        ui->messages->setItem(0, 1, i->item2);
        i->item1->setBackground(red);
       i->item2->setBackground(red);
       ui->messages->setItem(0, 0,  i->item1);
       ui->messages->setItem(0, 1, i->item2);
        message = QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+";";
        message.append("Ошибка насоса вод.охладителя");
        arh->write_fault_message(message,ch_unit);
    }
    if(((previous&2048)==2048)&((faults&2048)==0)){
        items_v[11]->item1->setBackground(white);
        items_v[11]->item2->setBackground(white);
    }


    if(((previous&4096)==0)&((faults&4096)==4096)){
        ui->messages->insertRow(0);
        items_str *i = new items_str;
        i->item1->setText(QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss"));
        i->item2->setText("Сработал датчик пожара");
        items_v[12]=i;
        ui->messages->setItem(0, 0,i->item1);
        ui->messages->setItem(0, 1, i->item2);
        i->item1->setBackground(red);
       i->item2->setBackground(red);
       ui->messages->setItem(0, 0,  i->item1);
       ui->messages->setItem(0, 1, i->item2);
        message = QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+";";
        message.append("Сработал датчик пожара");
        arh->write_fault_message(message,ch_unit);
    }
    if(((previous&4096)==4096)&((faults&4096)==0)){
        items_v[12]->item1->setBackground(white);
        items_v[12]->item2->setBackground(white);
    }


    if(((previous&8192)==0)&((faults&8192)==8192)){
        ui->messages->insertRow(0);
        items_str *i = new items_str;
        i->item1->setText(QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss"));
        i->item2->setText("Ошибка обрыва датчика");
        items_v[13]=i;
        ui->messages->setItem(0, 0,i->item1);
        ui->messages->setItem(0, 1, i->item2);
        i->item1->setBackground(red);
       i->item2->setBackground(red);
       ui->messages->setItem(0, 0,  i->item1);
       ui->messages->setItem(0, 1, i->item2);
        message = QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+";";
        message.append("Ошибка обрыва датчика");
        arh->write_fault_message(message,ch_unit);
    }
    if(((previous&8192)==8192)&((faults&8192)==0)){
        items_v[13]->item1->setBackground(white);
        items_v[13]->item2->setBackground(white);
    }


    if(((previous&16384)==0)&((faults&16384)==16384)){
        ui->messages->insertRow(0);
        items_str *i = new items_str;
        i->item1->setText(QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss"));
        i->item2->setText("Обрыв связи");
        items_v[14]=i;
        ui->messages->setItem(0, 0,i->item1);
        ui->messages->setItem(0, 1, i->item2);
        i->item1->setBackground(red);
       i->item2->setBackground(red);
       ui->messages->setItem(0, 0,  i->item1);
       ui->messages->setItem(0, 1, i->item2);
        message = QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+";";
        message.append("Обрыв связи");
        arh->write_fault_message(message,ch_unit);
    }
    if(((previous&16384)==16384)&((faults&16384)==0)){
        items_v[14]->item1->setBackground(white);
        items_v[14]->item2->setBackground(white);
    }


    if(((previous&32768)==0)&((faults&32786)==32786)){
        ui->messages->insertRow(0);
        items_str *i = new items_str;
        i->item1->setText(QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss"));
        i->item2->setText("Нет воды");
        items_v[15]=i;
        ui->messages->setItem(0, 0,i->item1);
        ui->messages->setItem(0, 1, i->item2);
        i->item1->setBackground(red);
       i->item2->setBackground(red);
       ui->messages->setItem(0, 0,  i->item1);
       ui->messages->setItem(0, 1, i->item2);
        message = QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+";";
        message.append("Нет воды");
        arh->write_fault_message(message,ch_unit);
    }
    if(((previous&32768)==32786)&((faults&32786)==0)){
        items_v[15]->item1->setBackground(white);
        items_v[15]->item2->setBackground(white);
    }


    if(((previous&65536)==0)&((faults&65536)==65536)){
        ui->messages->insertRow(0);
        items_str *i = new items_str;
        i->item1->setText(QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss"));
        i->item2->setText("Ошибка фильтра притока");
        items_v[16]=i;
        ui->messages->setItem(0, 0,i->item1);
        ui->messages->setItem(0, 1, i->item2);
        i->item1->setBackground(red);
       i->item2->setBackground(red);
       ui->messages->setItem(0, 0,  i->item1);
       ui->messages->setItem(0, 1, i->item2);
        message = QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+";";
        message.append("Ошибка фильтра притока");
        arh->write_fault_message(message,ch_unit);
    }
    if(((previous&65536)==65536)&((faults&65536)==0)){
        items_v[16]->item1->setBackground(white);
        items_v[16]->item2->setBackground(white);
    }


    if(((previous&131072)==0)&((faults&131072)==131072)){
        ui->messages->insertRow(0);
        items_str *i = new items_str;
        i->item1->setText(QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss"));
        i->item2->setText("Ошибка фильтра вытяжки");
        items_v[17]=i;
        ui->messages->setItem(0, 0,i->item1);
        ui->messages->setItem(0, 1, i->item2);
        i->item1->setBackground(red);
       i->item2->setBackground(red);
       ui->messages->setItem(0, 0,  i->item1);
       ui->messages->setItem(0, 1, i->item2);
        message = QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+";";
        message.append("Ошибка фильтра вытяжки");
        arh->write_fault_message(message,ch_unit);
    }
    if(((previous&131072)==131072)&((faults&131072)==0)){
        items_v[17]->item1->setBackground(white);
        items_v[17]->item2->setBackground(white);
    }


    if(((previous&262144)==0)&((faults&262144)==262144)){
        ui->messages->insertRow(0);
        items_str *i = new items_str;
        i->item1->setText(QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss"));
        i->item2->setText("Ошибка версии");
        items_v[18]=i;
        ui->messages->setItem(0, 0,i->item1);
        ui->messages->setItem(0, 1, i->item2);
        i->item1->setBackground(red);
       i->item2->setBackground(red);
       ui->messages->setItem(0, 0,  i->item1);
       ui->messages->setItem(0, 1, i->item2);
        message = QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+";";
        message.append("Ошибка версии");
        arh->write_fault_message(message,ch_unit);
    }
    if(((previous&262144)==262144)&((faults&262144)==0)){
        items_v[18]->item1->setBackground(white);
        items_v[18]->item2->setBackground(white);
    }


    if(((previous&524288)==0)&((faults&524288)==524288)){
        ui->messages->insertRow(0);
        items_str *i = new items_str;
        i->item1->setText(QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss"));
        i->item2->setText("Ошибка датчика рекуперации");
        items_v[19]=i;
        ui->messages->setItem(0, 0,i->item1);
        ui->messages->setItem(0, 1, i->item2);
        i->item1->setBackground(red);
       i->item2->setBackground(red);
       ui->messages->setItem(0, 0,  i->item1);
       ui->messages->setItem(0, 1, i->item2);
        message = QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+";";
        message.append("Ошибка датчика рекуперации");
        arh->write_fault_message(message,ch_unit);
    }
    if(((previous&524288)==524288)&((faults&524288)==0)){
        items_v[19]->item1->setBackground(white);
        items_v[19]->item2->setBackground(white);
    }


    if(((previous&1048576)==0)&((faults&1048576)==1048576)){
        ui->messages->insertRow(0);
        items_str *i = new items_str;
        i->item1->setText(QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss"));
        i->item2->setText("А20");
        items_v[20]=i;
        ui->messages->setItem(0, 0,i->item1);
        ui->messages->setItem(0, 1, i->item2);
        i->item1->setBackground(red);
       i->item2->setBackground(red);
       ui->messages->setItem(0, 0,  i->item1);
       ui->messages->setItem(0, 1, i->item2);
        message = QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+";";
        message.append("А20");
        arh->write_fault_message(message,ch_unit);
    }
    if(((previous&1048576)==1048576)&((faults&1048576)==0)){
        items_v[20]->item1->setBackground(white);
        items_v[20]->item2->setBackground(white);
    }


    if(((previous&2097152)==0)&((faults&2097152)==2097152)){
        ui->messages->insertRow(0);
        items_str *i = new items_str;
        i->item1->setText(QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss"));
        i->item2->setText("Ошибка связи с АТ");
        items_v[21]=i;
        ui->messages->setItem(0, 0,i->item1);
        ui->messages->setItem(0, 1, i->item2);
        i->item1->setBackground(red);
       i->item2->setBackground(red);
       ui->messages->setItem(0, 0,  i->item1);
       ui->messages->setItem(0, 1, i->item2);
        message = QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+";";
        message.append("Ошибка связи с АТ");
        arh->write_fault_message(message,ch_unit);
    }
    if(((previous&2097152)==2097152)&((faults&2097152)==0)){
        items_v[21]->item1->setBackground(white);
        items_v[21]->item2->setBackground(white);
    }


    if(((previous&4094304)==0)&((faults&4094304)==4094304)){
        ui->messages->insertRow(0);
        items_str *i = new items_str;
        i->item1->setText(QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss"));
        i->item2->setText("Переохлаждение канала!");
        items_v[22]=i;
        ui->messages->setItem(0, 0,i->item1);
        ui->messages->setItem(0, 1, i->item2);
        i->item1->setBackground(red);
       i->item2->setBackground(red);
       ui->messages->setItem(0, 0,  i->item1);
       ui->messages->setItem(0, 1, i->item2);
        message = QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+";";
        message.append("Переохлаждение канала!");
        arh->write_fault_message(message,ch_unit);
    }
    if(((previous&4094304)==4094304)&((faults&4094304)==0)){
        items_v[22]->item1->setBackground(white);
        items_v[22]->item2->setBackground(white);
    }


    if(((previous&8388608)==0)&((faults&8388608)==8388608)){
        ui->messages->insertRow(0);
        items_str *i = new items_str;
        i->item1->setText(QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss"));
        i->item2->setText("А23");
        items_v[23]=i;
        ui->messages->setItem(0, 0,i->item1);
        ui->messages->setItem(0, 1, i->item2);
        i->item1->setBackground(red);
       i->item2->setBackground(red);
       ui->messages->setItem(0, 0,  i->item1);
       ui->messages->setItem(0, 1, i->item2);
        message = QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+";";
        message.append("А23");
        arh->write_fault_message(message,ch_unit);
    }
    if(((previous&8388608)==8388608)&((faults&8388608)==0)){
        items_v[23]->item1->setBackground(white);
        items_v[23]->item2->setBackground(white);
    }


    if(((previous&16777216)==0)&((faults&16777216)==16777216)){
        ui->messages->insertRow(0);
        items_str *i = new items_str;
        i->item1->setText(QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss"));
        i->item2->setText("А24");
        items_v[24]=i;
        ui->messages->setItem(0, 0,i->item1);
        ui->messages->setItem(0, 1, i->item2);
        i->item1->setBackground(red);
       i->item2->setBackground(red);
       ui->messages->setItem(0, 0,  i->item1);
       ui->messages->setItem(0, 1, i->item2);
        message = QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+";";
        message.append("А24");
        arh->write_fault_message(message,ch_unit);
    }
    if(((previous&16777216)==16777216)&((faults&16777216)==0)){
        items_v[24]->item1->setBackground(white);
        items_v[24]->item2->setBackground(white);
    }


    if(((previous&33554432)==0)&((faults&33554432)==33554432)){
        ui->messages->insertRow(0);
        items_str *i = new items_str;
        i->item1->setText(QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss"));
        i->item2->setText("Trial: Срок истёк");
        items_v[25]=i;
        ui->messages->setItem(0, 0,i->item1);
        ui->messages->setItem(0, 1, i->item2);
        i->item1->setBackground(red);
       i->item2->setBackground(red);
       ui->messages->setItem(0, 0,  i->item1);
       ui->messages->setItem(0, 1, i->item2);
        message = QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+";";
        message.append("Trial: Срок истёк");
        arh->write_fault_message(message,ch_unit);
    }
    if(((previous&33554432)==33554432)&((faults&33554432)==0)){
        items_v[25]->item1->setBackground(white);
        items_v[25]->item2->setBackground(white);
    }


    if(((previous&67108864)==0)&((faults&67108864)==67108864)){
        ui->messages->insertRow(0);
        items_str *i = new items_str;
        i->item1->setText(QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss"));
        i->item2->setText("A26");
        items_v[26]=i;
        ui->messages->setItem(0, 0,i->item1);
        ui->messages->setItem(0, 1, i->item2);
        i->item1->setBackground(red);
       i->item2->setBackground(red);
       ui->messages->setItem(0, 0,  i->item1);
       ui->messages->setItem(0, 1, i->item2);
        message = QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+";";
        message.append("A26");
        arh->write_fault_message(message,ch_unit);
    }
    if(((previous&67108864)==67108864)&((faults&67108864)==0)){
        items_v[26]->item1->setBackground(white);
        items_v[26]->item2->setBackground(white);
    }


    if(((previous&134217728)==0)&((faults&134217728)==134217728)){
        ui->messages->insertRow(0);
        items_str *i = new items_str;
        i->item1->setText(QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss"));
        i->item2->setText("Авария Тобр 1+2");
        items_v[27]=i;
        ui->messages->setItem(0, 0,i->item1);
        ui->messages->setItem(0, 1, i->item2);
        i->item1->setBackground(red);
       i->item2->setBackground(red);
       ui->messages->setItem(0, 0,  i->item1);
       ui->messages->setItem(0, 1, i->item2);
        message = QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+";";
        message.append("Авария Тобр 1+2");
        arh->write_fault_message(message,ch_unit);
    }
    if(((previous&134217728)==134217728)&((faults&134217728)==0)){
        items_v[27]->item1->setBackground(white);
        items_v[27]->item2->setBackground(white);
    }


    if(((previous&268435456)==0)&((faults&268435456)==268435456)){
        ui->messages->insertRow(0);
        items_str *i = new items_str;
        i->item1->setText(QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss"));
        i->item2->setText("Нехватка EEPROM");
        items_v[28]=i;
        ui->messages->setItem(0, 0,i->item1);
        ui->messages->setItem(0, 1, i->item2);
        i->item1->setBackground(red);
       i->item2->setBackground(red);
       ui->messages->setItem(0, 0,  i->item1);
       ui->messages->setItem(0, 1, i->item2);
        message = QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+";";
        message.append("Нехватка EEPROM");
        arh->write_fault_message(message,ch_unit);
    }
    if(((previous&268435456)==268435456)&((faults&268435456)==0)){
        items_v[28]->item1->setBackground(white);
        items_v[28]->item2->setBackground(white);
    }


    if(((previous&536870912)==0)&((faults&536870912)==536870912)){
        ui->messages->insertRow(0);
        items_str *i = new items_str;
        i->item1->setText(QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss"));
        i->item2->setText("Авария Д(h)");
        items_v[29]=i;
        ui->messages->setItem(0, 0,i->item1);
        ui->messages->setItem(0, 1, i->item2);
        i->item1->setBackground(red);
       i->item2->setBackground(red);
       ui->messages->setItem(0, 0,  i->item1);
       ui->messages->setItem(0, 1, i->item2);
        message = QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+";";
        message.append("Авария Д(h)");
        arh->write_fault_message(message,ch_unit);
    }
    if(((previous&536870912)==536870912)&((faults&536870912)==0)){
        items_v[29]->item1->setBackground(white);
        items_v[29]->item2->setBackground(white);
    }


    if(((previous&1073741824)==0)&((faults&1073741824)==1073741824)){
        ui->messages->insertRow(0);
        items_str *i = new items_str;
        i->item1->setText(QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss"));
        i->item2->setText("Авария Д(CO2)");
        items_v[30]=i;
        ui->messages->setItem(0, 0,i->item1);
        ui->messages->setItem(0, 1, i->item2);
        i->item1->setBackground(red);
       i->item2->setBackground(red);
       ui->messages->setItem(0, 0,  i->item1);
       ui->messages->setItem(0, 1, i->item2);
        message = QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+";";
        message.append("Авария Д(CO2)");
        arh->write_fault_message(message,ch_unit);
    }
    if(((previous&1073741824)==1073741824)&((faults&1073741824)==0)){
        items_v[30]->item1->setBackground(white);
        items_v[30]->item2->setBackground(white);
    }
}
