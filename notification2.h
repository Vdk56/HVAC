#ifndef NOTIFICATION2_H
#define NOTIFICATION2_H

#include <QWidget>

namespace Ui {
class notification2;
}

class notification2 : public QWidget
{
    Q_OBJECT

public:
    explicit notification2(QWidget *parent = nullptr);
    ~notification2();

private slots:
    void on_pushButton_clicked();

private:
    Ui::notification2 *ui;
};

#endif // NOTIFICATION2_H
