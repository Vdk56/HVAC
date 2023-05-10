#ifndef AUTORIZATION_H
#define AUTORIZATION_H

#include <QWidget>
#include "notification3.h"


namespace Ui {
class autorization;
}

class autorization : public QWidget
{
    Q_OBJECT

public:
    explicit autorization(QWidget *parent = nullptr);
    ~autorization();
int status=0;
private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();
signals:
    void logi(int status);


private:
    Ui::autorization *ui;
    notification3 n;
};

#endif // AUTORIZATION_H
