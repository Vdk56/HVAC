#ifndef NOTIFICATION3_H
#define NOTIFICATION3_H

#include <QWidget>

namespace Ui {
class notification3;
}

class notification3 : public QWidget
{
    Q_OBJECT

public:
    explicit notification3(QWidget *parent = nullptr);
    ~notification3();

private slots:
    void on_pushButton_clicked();

private:
    Ui::notification3 *ui;
};

#endif // NOTIFICATION3_H
