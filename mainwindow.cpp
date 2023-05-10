#include "mainwindow.h"
#include "ui_mainwindow.h"

archive arch;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(add,SIGNAL(refresh()),this,SLOT(in_refr()));
    connect(w_ch,SIGNAL(signalGetM(int)), this,SLOT(chart_data_slot(int)));
    connect(this, SIGNAL(chart_data(int,int,int,int)),w_ch,SLOT(slotGetM(int,int,int,int)));
    connect(&v, SIGNAL(em(int,int,int)), this, SLOT(write(int,int,int)));
    connect(this, SIGNAL(statusesMW(QVector<int>,int,QVector<int>)),w_sch,SLOT(statuses(QVector<int>,int,QVector<int>)));
    connect(w_sch, SIGNAL(sigSch(int)), this, SLOT(send(int)));
    connect(&v, SIGNAL(unitS(int)), this, SLOT(send_unitState(int)));
    connect(this, SIGNAL(unitState(bool,bool,bool,bool,int)), &v, SLOT(unitStat(bool,bool,bool,bool,int)));
    connect(&aut,SIGNAL(logi(int)),this,SLOT(login(int)));
    ui->canc_but->hide();
    init();
    setEnabled(0);
    setDisabled(1);
    aut.show();
    t = new QTimer;
    t->setInterval(1000);
    connect(t,SIGNAL(timeout()),this,SLOT(timer()));
    t->start();
    tim = new QTimer;
    tim->setInterval(1500);
    connect(tim,SIGNAL(timeout()),this,SLOT(write_archive_sl()));
    tim->start();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::init()
{
    conf = new QFile("config.cfg");
    conf->open(QIODevice::ReadOnly);
    QString content;
    GrB.clear();
    for(;!conf->atEnd();) content = content + conf->readLine();
    QJsonDocument jdoc = QJsonDocument::fromJson(content.toUtf8());
    QJsonObject jobj = jdoc.object();
    QJsonObject obj_reg;
    conf->close();
    QJsonArray arr_temp = jobj["Devices"].toArray();
    QJsonArray reg_arr;
    device_number=arr_temp.count();
    if(device_number>0){
         for(int i = 0;i<device_number;i++)
         {
               QJsonObject sub=arr_temp[i].toObject();
               refresh(sub["Name:"].toString(),sub["Address_ip:"].toString(),sub["Port:"].toString(),sub["Modbus Address:"].toString(),i+1);
         }
    }
}

void MainWindow::refresh(QString name, QString ip, QString port, QString addr, int dev_num)
{
    f.setBold(1);
    v_un= new QGroupBox;
    gr_l = new QGridLayout;
    temp_l = new QLabel;
    temp_lb = new QLabel;
    status_l = new QLabel;
    status_lb = new QLabel;
    state_l = new QLabel;
    state_lb = new QLabel;
    open_b  = new QPushButton;
    del_b = new QPushButton;

    temp_l->setText("t°C помещения");
    temp_l->setFont(f);
    temp_lb->setFont(f);
    temper_lab_v.push_back(temp_lb);

    status_l->setText("Состояние ВУ");
    status_l->setFont(f);
    status_lb->setFont(f);
    status_label_V.push_back(status_lb);

    state_l->setText("Соединение");
    state_l->setFont(f);
    state_lb->setFont(f);
    state_lab_v.push_back(state_lb);

    QString s = name+QString::number(dev_num);
    //даём имя кнопке что бы при открытии окна управления определять для какого устройства это окно открыто
    open_b->setObjectName("But_"+QString::number(dev_num));
    open_b->setText("Открыть");
    open_b->setStyleSheet(
                "QPushButton{"
                "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,stop: 0 #E0E0E0, stop: 1 #FFFFFF);"
                "border: 1px solid gray;"
                "border-radius: 5px;"
                "margin-top: 1ex;"
                "}"
                );
    del_b->setText("Удалить");
    del_b->setStyleSheet(
                "QPushButton{"
                "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,stop: 0 #E0E0E0, stop: 1 #FFFFFF);"
                "border: 1px solid gray;"
                "border-radius: 5px;"
                "margin-top: 1ex;"
                "}"
                );

    gr_l->addWidget(temp_l,0,0);
    gr_l->addWidget(temp_lb,0,1);
    gr_l->addWidget(status_l,1,0);
    gr_l->addWidget(status_lb,1,1);
    gr_l->addWidget(state_l,2,0);
    gr_l->addWidget(state_lb,2,1);
    gr_l->addWidget(del_b,3,0);
    gr_l->addWidget(open_b,3,1);

    v_un->setLayout(gr_l);
    v_un->setMinimumSize(300,150);
    v_un->setMaximumSize(300,150);
    v_un->setTitle(s);
    v_un->setTabletTracking(1);
    v_un->setStyleSheet(
               " QGroupBox::title{"
                " subcontrol-origin: margin;"
                   " subcontrol-position: top left;"
                   " left: 10px;"
                 "    top: -3px;"
                "    margin: 0 5px;"
                "}"
                                     "QGroupBox{"
                                     "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,stop: 0 #E0E0E0, stop: 1 #FFFFFF);"
                                     "border: 2px solid gray;"
                                     "border-radius: 15px;"
                                     "margin-top: 1ex;"
                                     "}"
                                      );
    modbusdevice = new QModbusTcpClient;
    modbusdevice->setObjectName("Modbus_"+QString::number(dev_num));
    ModBus_Dev_vector.push_back(modbusdevice);
    new_connection_tcp(name,ip,port,addr,dev_num);

    QString filename = "archive_"+QString::number(ModBus_Dev_vector.count())+".txt";
    dev_archive = new QFile(filename);
    archives_v.push_back(dev_archive);

    filename = "messages_"+QString::number(ModBus_Dev_vector.count())+".txt";
    messages = new QFile(filename);
    messages_v.push_back(messages);
    messages->open(QIODevice::ReadWrite);
    messages->close();

    archive = new QVector<QString>;
    for (int i = 0;i<23;i++) {
       archive->push_back("");
    }
    arch_data_strings_v.push_back(*archive);

    alarm_code_v.push_back(0);
    state_code_v.push_back(0);
    SCADA_code.push_back(0);
    SENS_code.push_back(0);
    CPU_code.push_back(0);

    unit_states_v = new  QVector<int>;
    for (int i = 0;i<14;i++) {
       unit_states_v->push_back(0);
    }
    states_of_units_v.push_back(*unit_states_v);

    unit_readings_v = new  QVector<int>;
    for (int i = 0;i<20;i++) {
       unit_readings_v->push_back(0);
    }
    readings_of_units_v.push_back(*unit_readings_v);

    input_status =new InpStat;
    InpStat_v.push_back(input_status);
    cls = new coils;
    coils_v.push_back(cls);
    hold_reg = new holdingReg;
    holdingReg_v.push_back(hold_reg);

    ui->verticalLayout->addWidget(v_un);
    GrB.push_back(v_un);

    Buttons_V.push_back(open_b);
    connect(open_b, SIGNAL(clicked()),&v,SLOT(show_unit()));
    del_butt_V.push_back(del_b);
    QString but_name = "delete;"+QString::number(del_butt_V.count()-1);
    del_butt_V.last()->setObjectName(but_name);
    del_butt_V.last()->hide();
    connect(del_b, SIGNAL(clicked()),this,SLOT(deleteDevice()));

}

void MainWindow::new_connection_tcp(QString name, QString ip, QString port, QString addr, int dev_num)
{
    if(ModBus_Dev_vector.last()->state() != QModbusDevice::ConnectedState)
    {
        const QUrl url = QUrl::fromUserInput(ip+":"+port);
        ModBus_Dev_vector.last()->setConnectionParameter(QModbusDevice::NetworkPortParameter, url.port());
        ModBus_Dev_vector.last()->setConnectionParameter(QModbusDevice::NetworkAddressParameter, url.host());
        ModBus_Dev_vector.last()->setTimeout(100);
        ModBus_Dev_vector.last()->connectDevice();
    }
}

void MainWindow::del_vectors(int i)
{
       temper_lab_v[i]->deleteLater();
       temper_lab_v.remove(i);

       status_label_V[i]->deleteLater();
       status_label_V.remove(i);

       state_lab_v[i]->deleteLater();
       state_lab_v.remove(i);

       ModBus_Dev_vector[i]->deleteLater();
       ModBus_Dev_vector.remove(i);

       archives_v[i]->remove();
       archives_v[i]->~QFile();
       archives_v.remove(i);

       messages_v[i]->remove();
       messages_v[i]->~QFile();
       messages_v.remove(i);
  
       for (int j=0;j<arch_data_strings_v[i].count();j++) {
                   arch_data_strings_v[i][j].~QString();
       }
       arch_data_strings_v[i].~QList();

       arch_data_strings_v.remove(i);
  
       alarm_code_v.remove(i);

       state_code_v.remove(i);

       SCADA_code.remove(i);

       SENS_code.remove(i);

       CPU_code.remove(i);

       states_of_units_v[i].~QVector();
       states_of_units_v.remove(i);

       readings_of_units_v[i].~QVector();
       readings_of_units_v.remove(i);

       InpStat_v[i]->~InpStat();
       InpStat_v.remove(i);

       coils_v[i]->~coils();
       coils_v.remove(i);

       holdingReg_v[i]->~holdingReg();
       holdingReg_v.remove(i);

       GrB[i]->~QGroupBox();
       GrB.remove(i);

       Buttons_V.remove(i);
       for(int k = i;k<Buttons_V.count();k++){
            QString ob_name = Buttons_V[k]->objectName();
            QStringList list = ob_name.split("_");
            int numb = list[1].toInt();
            numb -=1;
            QString name="But_"+QString::number(numb);
            Buttons_V[k]->setObjectName(name);
       }

       del_butt_V.remove(i);
       for(int k = i;k<del_butt_V.count();k++){
            QString ob_name = del_butt_V[k]->objectName();
            QStringList list = ob_name.split(";");
            int numb = list[1].toInt();
            numb -=1;
            QString name="delete;"+QString::number(numb);
            del_butt_V[k]->setObjectName(name);
       }
}

void MainWindow::write_archive_sl()
{
    QString datetime;
    QDate d=QDate::currentDate();
    QTime t = QTime::currentTime();
    datetime = d.toString("dd/MM/yyyy ")+t.toString("hh:mm:ss")+";";
    for (int i=0;i<ModBus_Dev_vector.count();i++) {
    arch_data_strings_v[i][0]=datetime;
    arch.write_file(arch_data_strings_v[i],i+1);
    }
}

void MainWindow::in_refr()
{
    conf = new QFile("config.cfg");
    conf->open(QIODevice::ReadOnly);
    QString content;
    for(;!conf->atEnd();) content = content + conf->readLine();
    QJsonDocument jdoc = QJsonDocument::fromJson(content.toUtf8());
    QJsonObject jobj = jdoc.object();
    conf->close();
    QJsonArray arr_temp = jobj["Devices"].toArray();
    device_number=arr_temp.count();
    QJsonObject sub=arr_temp[device_number-1].toObject();
    refresh(sub["Name:"].toString(),sub["Address_ip:"].toString(),sub["Port:"].toString(),sub["Modbus Address:"].toString(),device_number);
    first_cycle =1;
}

void MainWindow::on_add_but_clicked()
{
    add->show();
}

void MainWindow::requests(int dev_num, int req_num)
{
  if( ModBus_Dev_vector[dev_num]->state() == QModbusDevice::ConnectedState){
    state_lab_v[dev_num]->setText("Connected");
    state_lab_v[dev_num]->setStyleSheet("color:green");
    if(req_num==0){
        QModbusDataUnit readUnit(QModbusDataUnit::DiscreteInputs,14336,6);
        QModbusReply *lastRequest = ModBus_Dev_vector[dev_num]->sendReadRequest(readUnit,1);
        lastRequest = ModBus_Dev_vector[dev_num]->sendReadRequest(readUnit,1);
        lastRequest->setObjectName(QString::number(dev_num)+";inputStatus");
        if(!lastRequest->isFinished()) connect(lastRequest,SIGNAL(finished()), this, SLOT(responce0()));
        else lastRequest->deleteLater();
    }
    if(req_num==1){
        QModbusDataUnit readUnit(QModbusDataUnit::Coils,15360, 5);
        QModbusReply *lastRequest = ModBus_Dev_vector[dev_num]->sendReadRequest(readUnit,1);
        lastRequest = ModBus_Dev_vector[dev_num]->sendReadRequest(readUnit,1);
        lastRequest->setObjectName(QString::number(dev_num)+";coils");
        if(!lastRequest->isFinished()) connect(lastRequest,SIGNAL(finished()), this, SLOT(responce1()));
        else lastRequest->deleteLater();
    }
    if(req_num==2){
        QModbusDataUnit readUnit(QModbusDataUnit::InputRegisters,40960, 21);
        QModbusReply *lastRequest = ModBus_Dev_vector[dev_num]->sendReadRequest(readUnit,1);
        lastRequest = ModBus_Dev_vector[dev_num]->sendReadRequest(readUnit,1);
        lastRequest->setObjectName(QString::number(dev_num)+";inputRegisters1");
        if(!lastRequest->isFinished()) connect(lastRequest,SIGNAL(finished()), this, SLOT(responce2()));
        else lastRequest->deleteLater();
    }
    if(req_num==3){
        QModbusDataUnit readUnit(QModbusDataUnit::InputRegisters,40993, 114);
        QModbusReply *lastRequest = ModBus_Dev_vector[dev_num]->sendReadRequest(readUnit,1);
        lastRequest = ModBus_Dev_vector[dev_num]->sendReadRequest(readUnit,1);
        lastRequest->setObjectName(QString::number(dev_num)+";inputRegisters2");
        if(!lastRequest->isFinished()) connect(lastRequest,SIGNAL(finished()), this, SLOT(responce3()));
        else lastRequest->deleteLater();
    }
    if(req_num==4){
        QModbusDataUnit readUnit(QModbusDataUnit::HoldingRegisters,41984, 9);
        QModbusReply *lastRequest = ModBus_Dev_vector[dev_num]->sendReadRequest(readUnit,1);
        lastRequest = ModBus_Dev_vector[dev_num]->sendReadRequest(readUnit,1);
        lastRequest->setObjectName(QString::number(dev_num)+";holdingRegisters");
        if(!lastRequest->isFinished()) connect(lastRequest,SIGNAL(finished()), this, SLOT(responce4()));
        else lastRequest->deleteLater();
    }
  }else{
      state_lab_v[dev_num]->setText("Con. failure");
      state_lab_v[dev_num]->setStyleSheet("color:red");
      ModBus_Dev_vector[dev_num]->connectDevice();
  }
}

void MainWindow::timer()
{
    for (int j =0;j<ModBus_Dev_vector.count();j++) {
            for (int i = 0;i<5;i++) {
               requests(j,i);
            }
    }
    first_cycle=0;
}

void MainWindow::responce0()
{
    QModbusReply *lastRequest = qobject_cast <QModbusReply *> (sender());
    if (!lastRequest) return;
    QStringList sl=lastRequest->objectName().split(";");
    int dev_num = sl[0].toInt();
     if(lastRequest->error() == QModbusDevice::NoError)
     {
         QModbusDataUnit unit = lastRequest->result();
          InpStat_v[dev_num]->SCo_WintSum = unit.value(0);
          InpStat_v[dev_num]->SCo_RemLoc = unit.value(1);
          InpStat_v[dev_num]->SCo_timer = unit.value(2);
          InpStat_v[dev_num]->SCo_lock = unit.value(3);
          InpStat_v[dev_num]->SCo_remStartStop = unit.value(4);
          InpStat_v[dev_num]->SCo_locStartStop = unit.value(5);
     }
}

void MainWindow::responce1()
{
    QModbusReply *lastRequest = qobject_cast <QModbusReply *> (sender());
    if (!lastRequest) return;
    QStringList sl=lastRequest->objectName().split(";");
    int dev_num = sl[0].toInt();
     if(lastRequest->error() == QModbusDevice::NoError)
     {
         QModbusDataUnit unit = lastRequest->result();
         coils_v[dev_num]->SCi_RemLoc = unit.value(0);
         coils_v[dev_num]->SCi_timer = unit.value(1);
         coils_v[dev_num]->SCi_Start = unit.value(2);
         coils_v[dev_num]->SCi_Stop = unit.value(3);
         coils_v[dev_num]->SCi_faultRes = unit.value(4);
     }
}

void MainWindow::responce2()
{
    QModbusReply *lastRequest = qobject_cast <QModbusReply *> (sender());
    if (!lastRequest) return;
    QStringList sl=lastRequest->objectName().split(";");
    QString s = "";
    int dev_num = sl[0].toInt();
     if(lastRequest->error() == QModbusDevice::NoError)
     {
         QModbusDataUnit unit = lastRequest->result();
         state_code_v[dev_num]=unit.value(12);
         switch (state_code_v[dev_num]) {
         case 1:
             status_label_V[dev_num]->setText("Останов");
             break;
         case 4:
             status_label_V[dev_num]->setText("Блокировка");
             break;
        case 8:
             status_label_V[dev_num]->setText("Продувка");
             break;
        case 16:
             status_label_V[dev_num]->setText("Прогрев");
             break;
        case 32:
             status_label_V[dev_num]->setText("Жалюзи");
             break;
        case 64:
             status_label_V[dev_num]->setText("Вентилятор");
             break;
        case 512:
             status_label_V[dev_num]->setText("Перезапуск");
             break;
         case 1024:
             status_label_V[dev_num]->setText("Работа");
             break;
         case 4096:
             status_label_V[dev_num]->setText("Сброс аварии");
             break;
         }

         quint16 temp1 = unit.value(4);
         quint16 temp2 = unit.value(5);
         quint32 final = temp2<<16|temp1;
         SCADA_code[dev_num]=final;

         temp1 = unit.value(8);
         temp2 = unit.value(9);
         final = temp2<<16|temp1;
         alarm_code_v[dev_num]=final;

         temp1 = unit.value(10);
         temp2 = unit.value(11);
         final = temp2<<16|temp1;

         SENS_code[dev_num]=final;

         temp1 = unit.value(6);
         temp2 = unit.value(7);
         final = temp2<<16|temp1;
         CPU_code[dev_num]=final;
         arch.write_scada_cod(SCADA_code);
         arch.write_sens_cod(SENS_code);
         arch.write_cpu_cod(CPU_code);
     }
}

void MainWindow::responce3()
{
     QModbusReply *lastRequest = qobject_cast <QModbusReply *> (sender());
     if (!lastRequest) return;
     QStringList sl=lastRequest->objectName().split(";");
     QString s = "";
     int dev_num = sl[0].toInt();
     if(lastRequest->error() == QModbusDevice::NoError){

         QModbusDataUnit unit = lastRequest->result();

         arch_data_strings_v[dev_num][1]=QString::number(unit.value(0))+";";
         arch_data_strings_v[dev_num][2]=QString::number(unit.value(2))+";";
         arch_data_strings_v[dev_num][3]=QString::number(unit.value(4))+";";
         arch_data_strings_v[dev_num][4]=QString::number(unit.value(6))+";";

         temper_lab_v[dev_num]->setStyleSheet("color:green");
         temper_lab_v[dev_num]->setText(QString::number(unit.value(6))+"°C");
         if(unit.value(6)>45) temper_lab_v[dev_num]->setStyleSheet("color:red");
         if(unit.value(6)<5) temper_lab_v[dev_num]->setStyleSheet("color:blue");

         arch_data_strings_v[dev_num][5]=QString::number(unit.value(8))+";";
         arch_data_strings_v[dev_num][6]=QString::number(unit.value(10))+";";
         arch_data_strings_v[dev_num][7]=QString::number(unit.value(12))+";";
         arch_data_strings_v[dev_num][8]=QString::number(unit.value(14))+";";
         arch_data_strings_v[dev_num][9]=QString::number(unit.value(16))+";";
         arch_data_strings_v[dev_num][10]=QString::number(unit.value(18))+";";
         arch_data_strings_v[dev_num][11]=QString::number(unit.value(20))+";";
         arch_data_strings_v[dev_num][12]=QString::number(unit.value(30))+";";
         arch_data_strings_v[dev_num][13]=QString::number(unit.value(32))+";";
         arch_data_strings_v[dev_num][14]=QString::number(unit.value(34))+";";
         arch_data_strings_v[dev_num][15]=QString::number(unit.value(40))+";";
         arch_data_strings_v[dev_num][16]=QString::number(unit.value(108))+";";
         arch_data_strings_v[dev_num][17]=QString::number(unit.value(109))+";";
         arch_data_strings_v[dev_num][18]=QString::number(unit.value(110))+";";
         arch_data_strings_v[dev_num][19]=QString::number(unit.value(111))+";";
         arch_data_strings_v[dev_num][20]=QString::number(unit.value(112))+";";
         arch_data_strings_v[dev_num][21]=QString::number(unit.value(113))+";";

         readings_of_units_v[dev_num][0]=unit.value(0);
         readings_of_units_v[dev_num][1]=unit.value(2);
         readings_of_units_v[dev_num][2]=unit.value(4);
         readings_of_units_v[dev_num][2]=unit.value(6);
         readings_of_units_v[dev_num][3]=unit.value(8);
         readings_of_units_v[dev_num][4]=unit.value(10);
         readings_of_units_v[dev_num][5]=unit.value(12);
         readings_of_units_v[dev_num][6]=unit.value(14);
         readings_of_units_v[dev_num][7]=unit.value(16);
         readings_of_units_v[dev_num][8]=unit.value(18);
         readings_of_units_v[dev_num][9]=unit.value(20);
         readings_of_units_v[dev_num][10]=unit.value(22);
         readings_of_units_v[dev_num][11]=unit.value(24);
         readings_of_units_v[dev_num][12]=unit.value(26);
         readings_of_units_v[dev_num][13]=unit.value(34);
         readings_of_units_v[dev_num][14]=unit.value(108);
         readings_of_units_v[dev_num][15]=unit.value(109);
         readings_of_units_v[dev_num][16]=unit.value(110);
         readings_of_units_v[dev_num][17]=unit.value(111);
         readings_of_units_v[dev_num][18]=unit.value(112);
         readings_of_units_v[dev_num][19]=unit.value(113);

         for (int i =87;i<101;i++) {
             states_of_units_v[dev_num][i-87]=unit.value(i);
         }
     }
}

void MainWindow::responce4()
{
     QModbusReply *lastRequest = qobject_cast <QModbusReply *> (sender());
     if (!lastRequest) return;
     QStringList sl=lastRequest->objectName().split(";");
     int dev_num = sl[0].toInt();
     if(lastRequest->error() == QModbusDevice::NoError)
     {
         QModbusDataUnit unit = lastRequest->result();
         holdingReg_v[dev_num]->SCi_ref_t = unit.value(0);
         holdingReg_v[dev_num]->SCi_ref_h = unit.value(2);
         holdingReg_v[dev_num]->SCi_ref_co2 = unit.value(4);
         holdingReg_v[dev_num]->SCi_ref_change_ws = unit.value(6);
         holdingReg_v[dev_num]->SCi_ref_season = unit.value(7);
         holdingReg_v[dev_num]->SCi_ref_behave = unit.value(8);
         holdingReg_v[dev_num]->Sci_fan_res = unit.value(9);
     }
}

void MainWindow::deleteDevice()
{
 t->stop();
 tim->stop();
 QStringList obj_name = sender()->objectName().split(";");
 qDebug()<<obj_name[1];
 del_vectors(obj_name[1].toInt());
 conf = new QFile("config.cfg");
 conf->open(QIODevice::ReadOnly);
 QString content;
 for(;!conf->atEnd();) content = content + conf->readLine();
 QJsonDocument jdoc = QJsonDocument::fromJson(content.toUtf8());
 QJsonObject sub_object;
 QJsonObject jobj = jdoc.object();
 conf->close();
 QJsonArray arr_temp = jobj["Devices"].toArray();
 arr_temp.removeAt(obj_name[1].toInt());
 jobj["Devices"]=arr_temp;
 QByteArray b_a = QJsonDocument(jobj).toJson();
 conf->open(QIODevice::WriteOnly);
 conf->write(b_a);
 conf->close();
 t->start();
 tim->start();
}

void MainWindow::send(int dev)
{
    if(ModBus_Dev_vector.count()==0)return;
    emit statusesMW(states_of_units_v[dev], alarm_code_v[dev],readings_of_units_v[dev]);
}

void MainWindow::chart_data_slot(int u)
{
    if(ModBus_Dev_vector.count()==0)return;
    QStringList s1= arch_data_strings_v[u][1].split(";");
     t11  =s1[0].toInt();
     s1= arch_data_strings_v[u][2].split(";");
     t22  =s1[0].toInt();
     s1= arch_data_strings_v[u][3].split(";");
     t33  =s1[0].toInt();
     s1= arch_data_strings_v[u][4].split(";");
     t44  =s1[0].toInt();
     emit chart_data(t11,t22,t33,t44);
}

void MainWindow::write(int dev,int type, int value)
{
    if(ModBus_Dev_vector.count()==0)return;

if( ModBus_Dev_vector[dev-1]->state() == QModbusDevice::ConnectedState){
   int i = dev-1;
   if(type==0){
        QModbusDataUnit writeUnit0 (QModbusDataUnit::HoldingRegisters, 41984,1);
        writeUnit0.setValue(0,value);
        QModbusReply *lastResponce0 = ModBus_Dev_vector[i]->sendWriteRequest(writeUnit0, 1);
        if (lastResponce0->isFinished()){
        } else lastResponce0->deleteLater();
   }
   if(type==1){
        QModbusDataUnit writeUnit1 (QModbusDataUnit::HoldingRegisters, 41986,1);
        writeUnit1.setValue(0,value);
        QModbusReply *lastResponce1 = ModBus_Dev_vector[i]->sendWriteRequest(writeUnit1, 1);
        if (lastResponce1->isFinished()){
        } else lastResponce1->deleteLater();
   }
   if(type==2){
        QModbusDataUnit writeUnit2 (QModbusDataUnit::HoldingRegisters, 41990,1);
        writeUnit2.setValue(0,value);
        QModbusReply *lastResponce2 = ModBus_Dev_vector[i]->sendWriteRequest(writeUnit2, 1);
        if (lastResponce2->isFinished()){
        } else lastResponce2->deleteLater();
    }
   if(type==3){
        QModbusDataUnit writeUnit3 (QModbusDataUnit::Coils, 15362,1);
        writeUnit3.setValue(0,value);
        QModbusReply *lastResponce3 = ModBus_Dev_vector[i]->sendWriteRequest(writeUnit3, 1);
        if (lastResponce3->isFinished()){
        } else lastResponce3->deleteLater();
   }
   if(type==4){
        QModbusDataUnit writeUnit4 (QModbusDataUnit::Coils, 15363,1);
        writeUnit4.setValue(0,value);
        QModbusReply *lastResponce4 = ModBus_Dev_vector[i]->sendWriteRequest(writeUnit4, 1);
        if (lastResponce4->isFinished()){
        } else lastResponce4->deleteLater();
    }
    if(type==5){
        QModbusDataUnit writeUnit5 (QModbusDataUnit::Coils, 15360,1);
        writeUnit5.setValue(0,value);
        QModbusReply *lastResponce5 = ModBus_Dev_vector[i]->sendWriteRequest(writeUnit5, 1);
        if (lastResponce5->isFinished()){
        } else lastResponce5->deleteLater();
     }
    if(type==6){
        QModbusDataUnit writeUnit6 (QModbusDataUnit::Coils, 15364,1);
        writeUnit6.setValue(0,value);
        QModbusReply *lastResponce6 = ModBus_Dev_vector[i]->sendWriteRequest(writeUnit6, 1);
        if (lastResponce6->isFinished()){
        } else lastResponce6->deleteLater();
    }
 }
}

void MainWindow::send_unitState(int dev)
{

    if(ModBus_Dev_vector.count()==0)return;

    int i = dev-1;
    bool oper;
    bool zl= InpStat_v[i]->SCo_WintSum;
    bool locRem= InpStat_v[i]->SCo_RemLoc;
    bool fault= InpStat_v[i]->SCo_lock;
    if(locRem) oper= InpStat_v[i]->SCo_remStartStop;
    else oper =  InpStat_v[i]->SCo_locStartStop;
    emit unitState( oper,zl,  locRem,  fault, alarm_code_v[dev-1]);
}

void MainWindow::login(int stat)
{
    if(stat==3) {this->close();}
    if(stat==2) {ui->del_butt->setDisabled(1); ui->add_but->setDisabled(1);setEnabled(1); setDisabled(0);}
    if(stat==1){setEnabled(1); setDisabled(0);}
}

void MainWindow::on_del_butt_clicked()
{
    if(ModBus_Dev_vector.count()==0)return;
    ui->canc_but->show();
    for (int i=0;i<del_butt_V.count();i++) {
        del_butt_V[i]->show();
    }
}

void MainWindow::on_canc_but_clicked()
{
    for (int i=0;i<del_butt_V.count();i++) {
        del_butt_V[i]->hide();
    }
    ui->canc_but->hide();
}
