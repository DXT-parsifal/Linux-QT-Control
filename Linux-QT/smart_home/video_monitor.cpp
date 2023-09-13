#include "video_monitor.h"
#include "ui_video_monitor.h"

#include <QFileDialog>
#include <QDebug>
Video_Monitor::Video_Monitor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Video_Monitor)
{
    ui->setupUi(this);
    ui->pushButton_photo->setEnabled(false);
    ui->label_photo->setScaledContents(true);
    ui->pushButton_save->setEnabled(false);
    connect(ui->pushButton_open,&QPushButton::clicked,this,&Video_Monitor::on_openBt_clicked);
    connect(ui->pushButton_close,&QPushButton::clicked,this,&Video_Monitor::on_closeBt_clicked);
    connect(ui->pushButton_photo,&QPushButton::clicked,this,&Video_Monitor::take_photo);
    connect(ui->pushButton_save,&QPushButton::clicked,this,[=](){
        QString path=QFileDialog::getSaveFileName(this,"save path","/home/root","*.jpg");
        if(!path.isEmpty()){
            this->photo.save(path);
        }else{
            qDebug()<<"select path";
        }
    });
}

Video_Monitor::~Video_Monitor()
{
    delete ui;
}

void Video_Monitor::take_photo()//拍照
{
    ui->label_photo->setPixmap(this->photo);
    ui->pushButton_save->setEnabled(true);
}

void Video_Monitor::on_openBt_clicked()
{
    connect(&vapi, &V4l2Api::sendImage, this, &Video_Monitor::recvImage);
    vapi.start();
}

void Video_Monitor::recvImage(QImage image)
{
    QPixmap mmp = QPixmap::fromImage(image);
    ui->label_video->setPixmap(mmp);

    ui->pushButton_photo->setEnabled(true);
    this->photo = QPixmap::fromImage(image);
}

void Video_Monitor::on_closeBt_clicked()
{
    disconnect(&vapi, &V4l2Api::sendImage, this, &Video_Monitor::recvImage);
    ui->label_video->clear();

    ui->pushButton_photo->setEnabled(false);
    ui->pushButton_save->setEnabled(false);
}

