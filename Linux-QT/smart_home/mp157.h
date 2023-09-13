#ifndef MP157_H
#define MP157_H

#include <QWidget>
#include "func.h"
#include <QDebug>
#include <QTimer>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QFile>
#include <QAudioInput>
//软键盘
#include "keyboard.h"
namespace Ui {
class MP157;
}

/*********主要做了MP157的媒体播放功能*******************/
class MP157 : public QWidget
{
    Q_OBJECT

public:
    explicit MP157(QWidget *parent = nullptr);
    ~MP157();
    //解决ID帐号问题
    void set_ID(QString setID);
    //************8888********多媒体

public slots:
    void addsongs();//添加歌曲
    void clearList();//清除歌曲
    void deleteList();//删除歌曲
    void playPause();//播放音乐
    void playNext();//下一曲
    void playPre();//上一曲
    void setVol(int vol);//设置声音
    void upDateRote();//更新旋转角度
    void posChange(qint64 pos);
    void durChanged(qint64 dur);
    //******************************在线搜歌曲************************/
    void slot_btn_request();//在线歌曲请求URL get
    void slots_http_finshed(QNetworkReply *reply);//请求回复读取JSON

protected:
    void paintEvent(QPaintEvent *event)override;//图片旋转绘画
    //-----------软键盘----------------------
    bool eventFilter(QObject *watched, QEvent *event);
private:
    Ui::MP157 *ui;
    QString ID;
    QTimer *timer;
    int flag_Image;
    //软件盘
    QLineEdit *lineEdit;
    void callKeyBoard();
    //***********************多媒体
    QMediaPlayer *player;
    QMediaPlaylist *playlist;
    int vol_flag;
    int flag;//动态是否开启
    QTimer *m_ptimer;
    double angle{0.0};
    bool m_bStart=true;
    QString presentTime;
    QString totalTime;

    //***************************************网络管理对象**********************/
    QNetworkAccessManager *http_online;
};

#endif // MP157_H
