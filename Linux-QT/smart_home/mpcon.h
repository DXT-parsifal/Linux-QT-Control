#ifndef MPCON_H
#define MPCON_H

#include <QDialog>
#include <QWidget>
#include "collentdatathread.h"
#include "Curtain.h"
#include "alarm.h"
namespace Ui {
class MPCON;
}

class MPCON : public QWidget
{
    Q_OBJECT

public:
    explicit MPCON(QWidget *parent = nullptr);
    ~MPCON();
public:
    void set_humAdtemAdill(QString,QString,QString);

public slots:
    void set_wake();//语音控制槽函数
private:
    Ui::MPCON *ui;
    int flag_led1;
    int flag_led2;
    int flag_led3;
    int flag_fan;
    Curtain *curtain;
    Alarm *alarm;
    //*************数据采集线程
    QString hum;
    QString tem;
    QString ill;
    QString hum_max;
    QString tem_max;
    QString ill_min;
    //数据采集线程
    CollentdataThread thread_collentdata;//线程
    int auto_monitor;
    QDialog *dlg;
};

#endif // MPCON_H
