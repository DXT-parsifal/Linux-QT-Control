#ifndef VIDEO_MONITOR_H
#define VIDEO_MONITOR_H

#include <QWidget>
#include <v4l2api.h>
namespace Ui {
class Video_Monitor;
}

class Video_Monitor : public QWidget
{
    Q_OBJECT

public:
    explicit Video_Monitor(QWidget *parent = nullptr);
    ~Video_Monitor();
private slots:
    void on_openBt_clicked();//打开摄像头
    void recvImage(QImage image);//读取图像
    void on_closeBt_clicked();//关闭摄像头
    void take_photo();//拍照记录
private:
    Ui::Video_Monitor *ui;
    V4l2Api vapi;
    QPixmap photo;
};

#endif // VIDEO_MONITOR_H


