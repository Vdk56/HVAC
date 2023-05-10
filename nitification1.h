#ifndef NITIFICATION1_H
#define NITIFICATION1_H

#include <QWidget>

namespace Ui {
class nitification1;
}

class nitification1 : public QWidget
{
    Q_OBJECT

public:
    explicit nitification1(QWidget *parent = nullptr);
    ~nitification1();

private slots:
    void on_pushButton_clicked();

private:
    Ui::nitification1 *ui;
};

#endif // NITIFICATION1_H
