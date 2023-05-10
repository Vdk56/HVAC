#include "widget_chart.h"
#include "ui_widget_chart.h"

int tt1;
int tt2;
int tt3;
int tt4;
int start;
int end1;
 int unit=0;
widget_chart::widget_chart(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::widget_chart)
{
    ui->setupUi(this);
    draw_chart();
    view.setChart(chart);
    view.setMouseTracking(1);
    view.viewport()->installEventFilter(this);
    ui->verticalLayout->addWidget(&view);
    t = new QTimer;
    tu=new QTimer;
    tu->setInterval(600);
    connect(tu,SIGNAL(timeout()),this,SLOT(timer_un()));
    t->setInterval(600);
    ui->dt_comp_1->setDateTime(QDateTime::currentDateTime());
    ui->dt_comp_2->setDateTime(QDateTime::currentDateTime());
    ui->dt_edit_go->setDateTime(QDateTime::currentDateTime());
    chart->setCursor(Qt::CrossCursor);
    connect(t,SIGNAL(timeout()),this,SLOT(chart_refresh()));
      tu->start();
    t->start();
}

widget_chart::~widget_chart()
{
    delete ui;
}

void widget_chart::draw_chart()
{
        ui->checkBox->setCheckState(Qt::Checked);
        ui->checkBox_2->setCheckState(Qt::Checked);
        ui->checkBox_3->setCheckState(Qt::Checked);
        ui->checkBox_4->setCheckState(Qt::Checked);

        QDateTime d = QDateTime::currentDateTime();

         chart->addSeries(series1);  // добавить серию на график
         chart->addSeries(series2);  // добавить серию на график
         chart->addSeries(series3);  // добавить серию на график
         chart->addSeries(series4);  // добавить серию на график

         chart->addAxis(axisX,Qt::AlignBottom);
         chart->addAxis(axisXX,Qt::AlignBottom);
         chart->addAxis(axisY,Qt::AlignLeft);

         series1->attachAxis(axisX);
         series1->attachAxis(axisXX);
         series1->attachAxis(axisY);
         connect(series1,SIGNAL(hovered(QPointF,bool)),this,SLOT(hover_ser(QPointF,bool)));

         series2->attachAxis(axisX);
         series2->attachAxis(axisXX);
         series2->attachAxis(axisY);
         connect(series2,SIGNAL(hovered(QPointF,bool)),this,SLOT(hover_ser(QPointF,bool)));

         series3->attachAxis(axisX);
         series3->attachAxis(axisXX);
         series3->attachAxis(axisY);

         connect(series3,SIGNAL(hovered(QPointF,bool)),this,SLOT(hover_ser(QPointF,bool)));

         series4->attachAxis(axisX);
         series4->attachAxis(axisXX);
         series4->attachAxis(axisY);
         connect(series4,SIGNAL(hovered(QPointF,bool)),this,SLOT(hover_ser(QPointF,bool)));

         axisX->setTickCount(10);
         axisX->setFormat("hh:mm:ss");
         axisX->setRange(d.addMSecs(-5000),d.addMSecs(6000));
         axisXX->setTickCount(1);
         axisXX->setFormat("dd MM yyyy");
         axisXX->setRange(d.addMSecs(-5000),d.addMSecs(6000));
         axisY->setRange(0,100);
}

int widget_chart::get_string_number(QString d)
{
    int i =strings_amount();
    file->open(QIODevice::ReadWrite);
     int am = 0;
      QString str;
    for(int j = 0;j<i;j++){
          str = file->readLine();
          QStringList list = str.split(";");
          QDateTime temp = QDateTime::fromString(list[0],"dd/MM/yyyy hh:mm:ss");
          if(d==temp.toString("dd/MM/yyyy hh:mm")){
              file->close();
              return am;
           }
          am++;
   }
    return am;
    file->close();
}

quint32 widget_chart::erliest_date()
{
    QString string;
    QString path = "archive_"+QString::number(unit+1)+".txt";
    file = new QFile(path);
    file->open(QIODevice::ReadWrite);
     QByteArray data;

     data = file->readLine();
     string = data;
     QStringList list = string.split(";");
     QDateTime d = QDateTime::fromString(list[0],"dd/MM/yyyy hh:mm:ss");
     return d.toSecsSinceEpoch();
}

bool widget_chart::eventFilter(QObject *target, QEvent *event)
{
    if (event->type() == QEvent::MouseMove){
          QMouseEvent *mouseEvent = (QMouseEvent *) event;
          chart->setCursor(Qt::CrossCursor);
         auto const widgetPos = mouseEvent->localPos();
         auto const scenePos = chart->mapToScene(QPoint(static_cast<int>(widgetPos.x()), static_cast<int>(widgetPos.y())));
        auto const chartItemPos = chart->mapFromScene(scenePos);
         auto const valueGivenSeries = chart->mapToValue(chartItemPos);
        qreal d= valueGivenSeries.x();
        
         QDateTime x = QDateTime::fromMSecsSinceEpoch(d);
          int y = valueGivenSeries.y();
          ui->coord_lab_x->setText(x.toString("dd/MM/yyyy hh:mm:ss"));
         ui->coord_lab_y->setText(QString::number(y)+" °C");
    }
}

QVector<QStringList> widget_chart::get_list(int start, int end)
{
    int str_am  = strings_amount();
    QString string;
    QString path = "archive_"+QString::number(unit+1)+".txt";
    file = new QFile(path);
    file->open(QIODevice::ReadWrite);
     QByteArray data;
    if(str_am<100){
        for(;!file->atEnd();){
            data = file->readLine();
            string = data;
            QStringList list = string.split(";");
            list_to_chart.push_back(list);
            list.clear();
            data.clear();
        }
    }
    else{
        for(int i = 0;i<str_am-start;i++){
             file->readLine();
        }
        for(int i = str_am-start;i<str_am-end;i++){
            data = file->readLine();
            string = data;
            QStringList list = string.split(";");
            list_to_chart.push_back(list);
            list.clear();
            data.clear();
        }
    }
    file->close();
    return     list_to_chart;
}

QVector<QStringList> widget_chart::get_list_for_goButton(int start, int end)
{
    QString path = "archive_"+QString::number(unit+1)+".txt";
    int str_am  = strings_amount();
    QString string;
    file = new QFile(path);
    file->open(QIODevice::ReadWrite);
     QByteArray data;
    if(str_am<100){
        for(;!file->atEnd();){
            data = file->readLine();
            string = data;
            QStringList list = string.split(";");
            list_to_chart.push_back(list);
            list.clear();
            data.clear();
        }
    }
    else{
        for(int i = 0;i<start;i++){
             file->readLine();
        }
        for(int i = 0;i<end-start;i++){
            data = file->readLine();
            string = data;
            QStringList list = string.split(";");
            list_to_chart.push_back(list);
            list.clear();
            data.clear();
        }
    }

    return     list_to_chart;
}

QVector<QStringList> widget_chart::get_list_for_compare_Button(int start, int end, int ser)
{
    QString path = "archive_"+QString::number(unit+1)+".txt";
    int str_am  = strings_amount();
    QString string;
    file = new QFile(path);
    file->open(QIODevice::ReadWrite);
     QByteArray data;
    if(str_am<100){
        for(;!file->atEnd();){
            data = file->readLine();
            string = data;
            QStringList list = string.split(";");
            list_to_chart.push_back(list);
            list.clear();
            data.clear();
        }
    }
    else{
        for(int i = 0;i<start;i++){
             file->readLine();
        }
        for(int i = 0;i<end-start;i++){
            data = file->readLine();
            string = data;
            QStringList list = string.split(";");
            list_to_chart.push_back(list);
            list.clear();
            data.clear();
        }
    }

    return     list_to_chart;
}

int widget_chart::strings_amount()
{
    int i = 0;
    QString filename = "archive_"+QString::number(unit+1)+".txt";
    file = new QFile(filename);
    file->open(QIODevice::ReadWrite);
    QString line;
    do{
        line = file->readLine();
        if(!line.isNull())  i++;

    }while(!line.isNull());
    qDebug()<<i;

     file->close();
    return i;
}

void widget_chart::set_dev(int i)
{

    unit = i-1;
}

void widget_chart::slotGetM(int t11,int t22,int t33, int t44)
{

    tt1=t11;
   tt2=t22;
    tt3=t33;
    tt4=t44;
}

void widget_chart::hover_ser(QPointF p, bool state)
{

    qreal d= p.x();
    QDateTime x = QDateTime::fromMSecsSinceEpoch(d);
    int y  = p.y();
    ui->coord_lab_x->setText(x.toString("dd/MM/yyyy hh:mm:ss"));
    ui->coord_lab_y->setText(QString::number(y));

}

void widget_chart::timer_un()
{

    emit signalGetM(unit);
}

void widget_chart::chart_refresh()
{      

        QDateTime d = QDateTime::currentDateTime();
        series1->append(d.toMSecsSinceEpoch(), tt1);
        series2->append(d.toMSecsSinceEpoch(),tt2);
        series3->append(d.toMSecsSinceEpoch(),tt3);
        series4->append(d.toMSecsSinceEpoch(),tt4);

          axisX->setRange(d.addMSecs(-5000),d.addMSecs(6000));
           axisXX->setRange(d.addMSecs(-5000),d.addMSecs(6000));
}

void widget_chart::on_arch_rec_b_clicked()
{

       tu->stop();
       t->stop();
       ui->back_but->setEnabled(1);
       ui->forw_but->setEnabled(1);
       ui->go_to_but->setEnabled(1);
       ui->dt_edit_go->setEnabled(1);
       ui->compare_but->setEnabled(1);
       ui->cb_comp1->setEnabled(1);
       ui->cb_comp2->setEnabled(1);
       ui->dt_comp_1->setEnabled(1);
       ui->dt_comp_2->setEnabled(1);
       ui->sb_duration->setEnabled(1);

       series1->clear();
       series2->clear();
       series3->clear();
       series4->clear();
       if(extra_axes){
            axisXcomp->deleteLater();
            axisXXcomp->deleteLater();
            extra_axes = 0;
       }

       start = 100;
       end1  = 1;
       QVector<QStringList> list = get_list(start,end1);
       for(int i = 0;i<list.count();i++){
            QStringList list_s = list[i];
            QDateTime d = QDateTime::fromString(list_s[0],"dd/MM/yyyy hh:mm:ss");
             series1->append(d.toMSecsSinceEpoch(), list_s[1].toInt());
            series2->append(d.toMSecsSinceEpoch(),list_s[2].toInt());
            series3->append(d.toMSecsSinceEpoch(),list_s[3].toInt());
            series4->append(d.toMSecsSinceEpoch(),list_s[4].toInt());
       }
       QDateTime d_r  = QDateTime::fromString(list[0][0],"dd/MM/yyyy hh:mm:ss");
        axisX->setRange(d_r,d_r.addMSecs(120000));
}

void widget_chart::on_record_but_clicked()
{
    tu->start();
    t->start();
    ui->back_but->setDisabled(1);
    ui->forw_but->setDisabled(1);
    ui->go_to_but->setDisabled(1);
    ui->dt_edit_go->setDisabled(1);
    ui->compare_but->setDisabled(1);
    ui->cb_comp1->setDisabled(1);
    ui->cb_comp2->setDisabled(1);
    ui->dt_comp_1->setDisabled(1);
    ui->dt_comp_2->setDisabled(1);

    series1->clear();
    series2->clear();
    series3->clear();
    series4->clear();
    QDateTime d_r  = QDateTime::currentDateTime();
     axisX->setRange(d_r,d_r.addMSecs(60000));
}

void widget_chart::on_checkBox_stateChanged(int arg1)
{
    if(ui->checkBox->isChecked()) series1->show();
    else series1->hide();
}

void widget_chart::on_checkBox_2_stateChanged(int arg1)
{
    if(ui->checkBox_2->isChecked()) series2->show();
    else series2->hide();
}

void widget_chart::on_checkBox_3_stateChanged(int arg1)
{
    if(ui->checkBox_3->isChecked()) series3->show();
    else series3->hide();
}

void widget_chart::on_checkBox_4_stateChanged(int arg1)
{
    if(ui->checkBox_4->isChecked()) series4->show();
    else series4->hide();
}

void widget_chart::on_compare_but_clicked()
{
 if(ui->sb_duration->value()==0) {notify1->show(); return;}
 quint32 a=erliest_date();
 quint32 b = ui->dt_comp_1->dateTime().toSecsSinceEpoch();
 quint32 c =  ui->dt_comp_2->dateTime().toSecsSinceEpoch();
 if((c < a)or(b < a)){
 notify->show();
 return;
 }

    axisXcomp = new QDateTimeAxis;
    axisXXcomp=new QDateTimeAxis;
    extra_axes = 1;
    QString date_1 = ui->dt_comp_1->dateTime().toString("dd/MM/yyyy hh:mm");
    QString date_2 = ui->dt_comp_2->dateTime().toString("dd/MM/yyyy hh:mm");
    int str_number_for_1  = get_string_number(date_1);
    int str_number_for_2 = get_string_number(date_2);
    interval_MINx5SEC_compare = ui->sb_duration->text().toInt()*42;

    series1->clear();
    series2->clear();
    series3->clear();
    series4->clear();
    compare_vector_1.clear();
    compare_vector_2.clear();

    QVector<QStringList> compare_vector_1 = get_list_for_goButton(str_number_for_1,str_number_for_1+interval_MINx5SEC_compare);

    for(int i = 0;i<compare_vector_1.count();i++){
        qDebug()<<"3.1";
         QStringList list_s = compare_vector_1[i];
         qDebug()<<list_s[0];
         QDateTime d = QDateTime::fromString(list_s[0],"dd/MM/yyyy hh:mm:ss");
         qDebug()<<"3.3";
          if(ui->cb_comp1->currentIndex()==0) {series1->append(d.toMSecsSinceEpoch(), list_s[1].toInt()); qDebug()<<"3.4";}
          if(ui->cb_comp1->currentIndex()==1) {series1->append(d.toMSecsSinceEpoch(), list_s[2].toInt()); qDebug()<<"3.5";}
          if(ui->cb_comp1->currentIndex()==2) {series1->append(d.toMSecsSinceEpoch(), list_s[3].toInt());qDebug()<<"3.6";}
          if(ui->cb_comp1->currentIndex()==3) {series1->append(d.toMSecsSinceEpoch(), list_s[4].toInt());qDebug()<<"3.7";}
    }

    QDateTime d_d  = QDateTime::fromString(compare_vector_1[0][0],"dd/MM/yyyy hh:mm:ss");
     axisX->setFormat("hh:mm:ss");
     axisX->setRange(d_d,d_d.addSecs(700));
     axisXX->setFormat("dd/MM/ hh:mm");
     axisXX->setTickCount(1);
     axisXX->setRange(d_d,d_d.addSecs(700));

     list_to_chart.clear();
     chart->addAxis(axisXcomp,Qt::AlignBottom);
     chart->addAxis(axisXXcomp,Qt::AlignBottom);
     series2->detachAxis(axisX);
     series2->detachAxis(axisXX);
     series2->attachAxis(axisXcomp);
     series2->attachAxis(axisXXcomp);
      axisXcomp->setTickCount(10);
      axisXXcomp->setTickCount(1);
     QVector<QStringList> compare_vector_2 = get_list_for_goButton(str_number_for_2,str_number_for_2+interval_MINx5SEC_compare);
     for(int i = 0;i<compare_vector_2.count();i++){
          QStringList list_s = compare_vector_2[i];
          QDateTime d = QDateTime::fromString(list_s[0],"dd/MM/yyyy hh:mm:ss");
           if(ui->cb_comp2->currentIndex()==0) series2->append(d.toMSecsSinceEpoch(), list_s[1].toInt());
           if(ui->cb_comp2->currentIndex()==1) series2->append(d.toMSecsSinceEpoch(), list_s[2].toInt());
           if(ui->cb_comp2->currentIndex()==2) series2->append(d.toMSecsSinceEpoch(), list_s[3].toInt());
           if(ui->cb_comp2->currentIndex()==3) series2->append(d.toMSecsSinceEpoch(), list_s[4].toInt());
     }

     QDateTime d_f  = QDateTime::fromString(compare_vector_2[0][0],"dd/MM/yyyy hh:mm:ss");

     axisXcomp->setFormat("hh:mm:ss");
     axisXcomp->setRange(d_f,d_f.addSecs(700));

     axisXXcomp->setFormat("dd/MM/ hh:mm");
     axisXXcomp->setRange(d_f,d_f.addSecs(700));
}

void widget_chart::on_back_but_clicked()
{
    if(strings_amount()<51) return;
    if (start>50){
        start +=50;
        end1  += 50;
    }
    else return;
    tu->stop();
    t->stop();
    ui->back_but->setEnabled(1);
    ui->forw_but->setEnabled(1);
    ui->go_to_but->setEnabled(1);
    ui->dt_edit_go->setEnabled(1);
    ui->compare_but->setEnabled(1);
    ui->cb_comp1->setEnabled(1);
    ui->cb_comp2->setEnabled(1);
    ui->dt_comp_1->setEnabled(1);
    ui->dt_comp_2->setEnabled(1);

        list_to_chart.clear();

    series1->clear();
    series2->clear();
    series3->clear();
    series4->clear();

    QVector<QStringList> list = get_list(start,end1);
    for(int i = 0;i<list.count();i++){
         QStringList list_s = list[i];
         QDateTime d = QDateTime::fromString(list_s[0],"dd/MM/yyyy hh:mm:ss");
          series1->append(d.toMSecsSinceEpoch(), list_s[1].toInt());
         series2->append(d.toMSecsSinceEpoch(),list_s[2].toInt());
         series3->append(d.toMSecsSinceEpoch(),list_s[3].toInt());
         series4->append(d.toMSecsSinceEpoch(),list_s[4].toInt());
    }
    QDateTime d_r  = QDateTime::fromString(list[0][0],"dd/MM/yyyy hh:mm:ss");
     axisX->setRange(d_r,d_r.addMSecs(120000));
}

void widget_chart::on_forw_but_clicked()
{
    if(end1==0) return;
    if(end1<50)
    {
         start -= -1*(0-end1);
        end1  = 0;
    }
    end1 -=50;
    start-=50;

    tu->stop();
    t->stop();
    ui->back_but->setEnabled(1);
    ui->forw_but->setEnabled(1);
    ui->go_to_but->setEnabled(1);
    ui->dt_edit_go->setEnabled(1);
    ui->compare_but->setEnabled(1);
    ui->cb_comp1->setEnabled(1);
    ui->cb_comp2->setEnabled(1);
    ui->dt_comp_1->setEnabled(1);
    ui->dt_comp_2->setEnabled(1);

    list_to_chart.clear();

    series1->clear();
    series2->clear();
    series3->clear();
    series4->clear();

    QVector<QStringList> list = get_list(start,end1);
    for(int i = 0;i<list.count();i++){
         QStringList list_s = list[i];
         QDateTime d = QDateTime::fromString(list_s[0],"dd/MM/yyyy hh:mm:ss");
          series1->append(d.toMSecsSinceEpoch(), list_s[1].toInt());
         series2->append(d.toMSecsSinceEpoch(),list_s[2].toInt());
         series3->append(d.toMSecsSinceEpoch(),list_s[3].toInt());
         series4->append(d.toMSecsSinceEpoch(),list_s[4].toInt());
    }
    QDateTime d_r  = QDateTime::fromString(list[0][0],"dd/MM/yyyy hh:mm:ss");
     axisX->setRange(d_r,d_r.addMSecs(120000));
}

void widget_chart::on_go_to_but_clicked()
{
    quint32 a=erliest_date();
    quint32 b = ui->dt_edit_go->dateTime().toSecsSinceEpoch();

    if(b < a){
       notify->show();
       return;
    }
     QDateTime d_r=ui->dt_edit_go->dateTime();
     QString dt_string = ui->dt_edit_go->dateTime().toString("dd/MM/yyyy hh:mm");
     int i = get_string_number(dt_string);
      if(i<1) return;
     if(i<50){
         start = 0;
          end1  =i+ 50;
    }else{
         start = i-50;
         end1  =i+ 50;
     }

     ui->back_but->setEnabled(1);
     ui->forw_but->setEnabled(1);
     ui->go_to_but->setEnabled(1);
     ui->dt_edit_go->setEnabled(1);
     ui->compare_but->setEnabled(1);
     ui->cb_comp1->setEnabled(1);
     ui->cb_comp2->setEnabled(1);
     ui->dt_comp_1->setEnabled(1);
     ui->dt_comp_2->setEnabled(1);

     list_to_chart.clear();

     series1->clear();
     series2->clear();
     series3->clear();
     series4->clear();

     QVector<QStringList> list = get_list_for_goButton(start,end1);
     for(int i = 0;i<list.count();i++){
          QStringList list_s = list[i];
          QDateTime d = QDateTime::fromString(list_s[0],"dd/MM/yyyy hh:mm:ss");
           series1->append(d.toMSecsSinceEpoch(), list_s[1].toInt());
          series2->append(d.toMSecsSinceEpoch(),list_s[2].toInt());
          series3->append(d.toMSecsSinceEpoch(),list_s[3].toInt());
          series4->append(d.toMSecsSinceEpoch(),list_s[4].toInt());
     }
     QDateTime d_d  = QDateTime::fromString(list[0][0],"dd/MM/yyyy hh:mm:ss");
      axisX->setRange(d_d,d_d.addMSecs(70000));
}

