#include "mpcon.h"
#include "ui_mpcon.h"
#include <QDebug>
#include <QHBoxLayout>
#include <QLineEdit>
MPCON::MPCON(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MPCON)
{
    ui->setupUi(this);
    flag_led1=0;
    flag_led2=0;
    flag_led3=0;
    ui->label_led_1->setPixmap(QPixmap(":/new/prefix1/res/close.png"));
    ui->label_led_2->setPixmap(QPixmap(":/new/prefix1/res/close.png"));
    ui->label_led_3->setPixmap(QPixmap(":/new/prefix1/res/close.png"));

    ui->label_led1->setPixmap(QPixmap(":/new/prefix1/res/灯具关.png"));
    ui->label_led2->setPixmap(QPixmap(":/new/prefix1/res/灯具关.png"));
    ui->label_led3->setPixmap(QPixmap(":/new/prefix1/res/灯具关.png"));
    //LDE1
    connect(ui->pushButton_led1,&QPushButton::clicked,this,[=](){
        if(0==flag_led1){
            system("echo 1 > /sys/class/leds/led1/brightness");
            ui->label_led1->setPixmap(QPixmap(":/new/prefix1/res/灯具开.png"));
            ui->label_led_1->setPixmap(QPixmap(":/new/prefix1/res/close.png"));
            flag_led1=1;
        }else if(1==flag_led1){
            system("echo 0 > /sys/class/leds/led1/brightness");
            ui->label_led1->setPixmap(QPixmap(":/new/prefix1/res/灯具关.png"));
            ui->label_led_1->setPixmap(QPixmap(":/new/prefix1/res/open.png"));
            flag_led1=0;
        }
    });
    //LED2
    connect(ui->pushButton_led2,&QPushButton::clicked,this,[=](){
        if(0==flag_led2){
            system("echo 1 > /sys/class/leds/led2/brightness");
            ui->label_led2->setPixmap(QPixmap(":/new/prefix1/res/灯具开.png"));
            ui->label_led_2->setPixmap(QPixmap(":/new/prefix1/res/close.png"));
            flag_led2=1;
        }else if(1==flag_led2){
            system("echo 0 > /sys/class/leds/led2/brightness");
            ui->label_led2->setPixmap(QPixmap(":/new/prefix1/res/灯具关.png"));
            ui->label_led_2->setPixmap(QPixmap(":/new/prefix1/res/open.png"));
            flag_led2=0;
        }
    });
    //LED3
    connect(ui->pushButton_led3,&QPushButton::clicked,this,[=](){
        if(0==flag_led3){
            system("echo 1 > /sys/class/leds/led3/brightness");
            ui->label_led3->setPixmap(QPixmap(":/new/prefix1/res/灯具开.png"));
            ui->label_led_3->setPixmap(QPixmap(":/new/prefix1/res/close.png"));
            flag_led3=1;
        }else if(1==flag_led3){
            system("echo 0 > /sys/class/leds/led3/brightness");
            ui->label_led3->setPixmap(QPixmap(":/new/prefix1/res/灯具关.png"));
            ui->label_led_3->setPixmap(QPixmap(":/new/prefix1/res/open.png"));
            flag_led3=0;
        }
    });

    //FAN
    this->flag_fan=0;
    ui->label_fan2->setPixmap(QPixmap(":/new/prefix1/res/close.png"));
    ui->label_fan1->setPixmap(QPixmap(":/new/prefix1/res/风扇.png"));
    connect(ui->pushButton_fan,&QPushButton::clicked,this,[=](){
        if(flag_fan==0){
           system("echo 100 > /sys/class/hwmon/hwmon1/pwm1");
           ui->label_fan1->setPixmap(QPixmap(":/new/prefix1/res/风扇1.png"));
           ui->label_fan2->setPixmap(QPixmap(":/new/prefix1/res/open.png"));
           flag_fan=1;
        }else if(flag_fan==1){
            system("echo 175 > /sys/class/hwmon/hwmon1/pwm1");
            ui->label_fan1->setPixmap(QPixmap(":/new/prefix1/res/风扇2.png"));
            ui->label_fan2->setPixmap(QPixmap(":/new/prefix1/res/open.png"));
            flag_fan=2;
        }else if(flag_fan==2){
            system("echo 255 > /sys/class/hwmon/hwmon1/pwm1");
            ui->label_fan1->setPixmap(QPixmap(":/new/prefix1/res/风扇3.png"));
            ui->label_fan2->setPixmap(QPixmap(":/new/prefix1/res/open.png"));
            flag_fan=3;
        }else if(flag_fan==3){
            system("echo 0 > /sys/class/hwmon/hwmon1/pwm1");
            ui->label_fan1->setPixmap(QPixmap(":/new/prefix1/res/风扇.png"));
            ui->label_fan2->setPixmap(QPixmap(":/new/prefix1/res/close.png"));
            flag_fan=0;
        }
    });

    //马达窗帘
    this->curtain=new Curtain();
    ui->label_curtain->setPixmap(QPixmap(":/new/prefix1/res/窗帘关.png"));
    ui->label_curtain1->setPixmap(QPixmap(":/new/prefix1/res/close.png"));
    connect(ui->pushButton_vibrator,&QPushButton::clicked,this,[=](){
        curtain->dev_start();
        qDebug()<<"vibrator start";
    });
    //加湿器
    ui->label_humidifler->setPixmap(QPixmap(":/new/prefix1/res/加湿器关.png"));
    ui->label_humidifler1->setPixmap(QPixmap(":/new/prefix1/res/close.png"));
    connect(ui->pushButton_hum,&QPushButton::clicked,this,[=](){

    });

    //紧急报警
    this->alarm = new Alarm();
    connect(ui->pushButton_beep,&QPushButton::clicked,this,[=](){
        alarm->alarm_control();
        qDebug()<<"beep";
    });
    /****************************** 启动数据采集线程  ****************************/
    //智能检测
    this->auto_monitor=0;
    this->hum_max="60";
    this->tem_max="30";
    this->ill_min="0";
    connect(ui->pushButton_auto_monitor,&QPushButton::clicked,this,[=](){
        if(auto_monitor==0){
            auto_monitor=1;//开启
        }else if(auto_monitor==1){
            auto_monitor=0;//关闭
        }
    });
    connect(&thread_collentdata,&CollentdataThread::send,this,&MPCON::set_humAdtemAdill);
    thread_collentdata.start();
    //数据显示
    ui->label_tem_hum_ill->setPixmap(QPixmap(":/new/prefix1/res/环境监测.jpg"));
    ui->label_tem_hum_ill->setScaledContents(true);

    //修改阈值
    connect(ui->pushButton_revise,&QPushButton::clicked,this,[=](){
        this->dlg=new QDialog(this);
        dlg->setWindowTitle("修改检测阈值");
        dlg->resize(400,300);
        QLabel *lab_tem = new QLabel("温度:",dlg);
        QLineEdit *line_tem = new QLineEdit(dlg);
        QVBoxLayout *vlayout1 = new QVBoxLayout(dlg);
        vlayout1->addWidget(lab_tem);
        vlayout1->addWidget(line_tem);

        QLabel *lab_ill = new QLabel("光照:",dlg);
        QLineEdit *line_ill = new QLineEdit(dlg);
        QVBoxLayout *vlayout2 = new QVBoxLayout(dlg);
        vlayout1->addWidget(lab_ill);
        vlayout1->addWidget(line_ill);

        QPushButton *dlg_Close = new QPushButton(dlg);
        dlg_Close->setText("提交");
        QHBoxLayout *layout=new QHBoxLayout(dlg);
        layout->addWidget(dlg_Close);
        layout->addLayout(vlayout1);
        layout->addLayout(vlayout2);
        dlg->setLayout(layout);
        dlg->show();
        connect(dlg_Close,&QPushButton::clicked,this,[=](){
            if(line_tem->text()!=""){
                this->tem_max=line_tem->text();
            }
            if(line_ill->text()!=""){
                this->ill_min=line_ill->text();
            }
            dlg->close();
            qDebug()<<"line_tem="<<line_tem->text()<<" tem_max="<<this->tem_max;
        });
    });

    //功能场景
    connect(ui->pushButton_wake,&QPushButton::clicked,this,[=](){//起床
        this->flag_led1=0;
        emit ui->pushButton_led1->clicked();
        emit ui->pushButton_vibrator->clicked();
    });
}

void MPCON::set_humAdtemAdill(QString tem, QString hum, QString ill)
{
    this->tem=tem;
    this->hum=hum;
    this->ill=ill;
    if(auto_monitor==1){
        //温度检测
        if(this->tem.toFloat()>this->tem_max.toFloat()){
            this->flag_fan=2;//三fan
            emit ui->pushButton_fan->clicked();
        }else {
            this->flag_fan=3;//关
            emit ui->pushButton_fan->clicked();
        }
//        //湿度检测
//        if(this->hum_max.toFloat()>this->hum_max.toFloat()){
//            this->flag_fan=0;//一fan
//            emit ui->pushButton_fan->clicked();
//        }else {
//            this->flag_fan=3;//关
//            emit ui->pushButton_fan->clicked();
//        }
        //灯光检测
        if(this->ill.toFloat() < 6.0){
            this->flag_led1=0;
            emit ui->pushButton_led1->clicked();
        }else {
            this->flag_led1=1;
            emit ui->pushButton_led1->clicked();
        }
        if(this->ill.toFloat() < 4.0){
            this->flag_led2=0;
            emit ui->pushButton_led2->clicked();
        }else {
            this->flag_led2=1;
            emit ui->pushButton_led2->clicked();
        }
        if(this->ill.toFloat()<2.0){
            this->flag_led3=0;
            emit ui->pushButton_led3->clicked();
        }else {
            this->flag_led3=1;
            emit ui->pushButton_led3->clicked();
        }

    }
    qDebug()<<"tem="<<tem<<" hum="<<hum<<" ill="<<ill;
    ui->label_hum->setText(this->hum);
    ui->label_ill->setText(this->ill);
    ui->label_tem->setText(this->tem);
}
//槽函数语音控制wake
void MPCON::set_wake()
{
    emit ui->pushButton_wake->clicked();
}

MPCON::~MPCON()
{
    delete alarm;
    delete curtain;
    delete ui;
}
