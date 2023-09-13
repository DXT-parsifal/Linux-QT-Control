#include "mp157.h"
#include "ui_mp157.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QPainter>

MP157::MP157(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MP157)
{
    ui->setupUi(this);
    //------------软键盘事件开启捕获-------------------
    ui->lineEdit_serch->installEventFilter(this);

    /*********************TAB1Theme设置*******************************/
    ui->label_Theme->setPixmap(QPixmap(":/new/prefix1/res/Theme.png"));
    this->timer = new QTimer(this);
    this->flag=1;
    this->flag_Image=0;
    connect(ui->pushButton_Theme_ch,&QPushButton::clicked,this,[=](){
        if(this->flag==1){//开启动态
            qDebug()<<"start";
            this->timer->start(1000);
            this->flag=0;
            ui->pushButton_Theme_ch->setText("关闭动态壁纸");
        }else if(this->flag==0){//关闭动态
            this->timer->stop();
            this->flag=1;
            ui->pushButton_Theme_ch->setText("切换动态壁纸");
        }
    });
    connect(timer,&QTimer::timeout,this,[=](){
        this->flag_Image++;
        if(this->flag_Image==3){
            ui->label_Theme->setPixmap(QPixmap(":/new/prefix1/res/team.png"));
        }else if(this->flag_Image==6){
            ui->label_Theme->setPixmap(QPixmap(":/new/prefix1/res/Theme.png"));
            this->flag_Image=0;
        }
    });

    /**************************medium媒体播放***********************/
    ui->pushButton_vol->setStyleSheet("background-image: url(:/new/prefix1/res/音乐播放器_04.png);");
    this->vol_flag=0;
    connect(ui->pushButton_vol,&QPushButton::clicked,this,[=](){//声音图片
        if(vol_flag==0){
            setVol(0);
            ui->horizontalSlider_vol->setValue(0);
            ui->pushButton_vol->setStyleSheet("background-image: url(:/new/prefix1/res/_09.png);");
            vol_flag=1;
        }else if(vol_flag==1){
            setVol(20);
            ui->horizontalSlider_vol->setValue(20);
            ui->pushButton_vol->setStyleSheet("background-image: url(:/new/prefix1/res/音乐播放器_04.png);");
            vol_flag=0;
        }
    });
    this->playlist = new QMediaPlaylist(this);
    playlist->setPlaybackMode(QMediaPlaylist::Loop);
    this->player = new QMediaPlayer(this);
    player->setPlaylist(playlist);
    player->setVolume(50);
    ui->horizontalSlider_vol->setValue(50);
    connect(ui->pushButton_add,&QPushButton::clicked,this,&MP157::addsongs);
    connect(ui->pushButton_ki,&QPushButton::clicked,this,&MP157::clearList);
    connect(ui->pushButton_delete,&QPushButton::clicked,this,&MP157::deleteList);
    ui->toolButton_play->setText(" ");
    ui->toolButton_play->setStyleSheet("background-image: url(:/new/prefix1/res/音乐播放器_06.png);");
    connect(ui->toolButton_play,&QToolButton::clicked,this,&MP157::playPause);
    ui->label_play_next->setStyleSheet("background-image: url(:/new/prefix1/res/音乐播放器_08.png);");
    ui->label_play_next->setScaledContents(true);
    connect(ui->toolButton_next,&QToolButton::clicked,this,&MP157::playNext);
    ui->label_play_pre->setScaledContents(true);
    ui->label_play_pre->setStyleSheet("background-image: url(:/new/prefix1/res/音乐播放器_07.png);");
    connect(ui->toolButton_prev,&QToolButton::clicked,this,&MP157::playPre);
    connect(ui->horizontalSlider_vol,&QSlider::valueChanged,this,&MP157::setVol);
    //录音和播放冲突问题
    connect(ui->widget_F4,&Func::playmedium,this,[=](int flag){
        if(0==flag){
            m_bStart=false;
            ui->toolButton_play->setText("\n");
            playPause();
        }
    });
    //播放图片旋转
    this->m_ptimer=new QTimer(this);
    connect(this->m_ptimer,&QTimer::timeout,this,&MP157::upDateRote);
    //音乐时长
    connect(player,&QMediaPlayer::durationChanged,this,&MP157::durChanged);
    connect(player,&QMediaPlayer::positionChanged,this,&MP157::posChange);

    //*********************************语音识别在线搜歌*****************************/
    this->http_online=new QNetworkAccessManager(this);//网络管理对象
    //http搜索歌曲名
    connect(ui->pushButton,&QPushButton::clicked,this,&MP157::slot_btn_request);
    //搜索结束后处理
    connect(http_online,&QNetworkAccessManager::finished,this,&MP157::slots_http_finshed);

    //传递语音识别的音乐名
    connect(ui->widget_F4,&Func::send_music_name,this,[=](QString music_name){
        ui->lineEdit_serch->setText(music_name);
    });
    //mpcon界面和func界面语音识别处理
    connect(ui->widget_F4,&Func::send_wake,ui->widget_2,&MPCON::set_wake);
}

//音乐ID请求API
void MP157::slot_btn_request()
{
    if(ui->lineEdit_serch->text().isEmpty()){
        return;
    }else{
        QString lineEdit_text=ui->lineEdit_serch->text();
        QString url="http://music.163.com/api/search/get/web?csrf_token=hlpretag=&hlposttag=&s=";
        url += lineEdit_text;
        url += "&type=1&offset=0&total=true&limit=2";
        //发送请求
        http_online->get(QNetworkRequest(QUrl(url)));
    }
}

void MP157::slots_http_finshed(QNetworkReply *reply)
{
    if(reply->error()==QNetworkReply::TimeoutError){
        QMessageBox::warning(this,"http:404","请求超时");
        return;
    }
    int stat=reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

    //读取全部接受到的信息
    QByteArray data=reply->readAll();
    if(stat!=200){//成功获取
        QMessageBox::warning(this,"http警告","请求失败，错误数据:"+data);
        return;
    }

    QJsonDocument document=QJsonDocument::fromJson(data);
    QJsonObject jsonobject=document.object();
    QJsonObject object_result =jsonobject.value("result").toObject();
    QJsonArray array_songs =object_result.value("songs").toArray();
    QJsonObject object_song=array_songs.at(0).toObject();

    //音乐name
    QString music_name=object_song["name"].toString();
    ui->listWidget_songs->addItem(music_name);
    //音乐ID
    QString music_id="http://music.163.com/song/media/outer/url?id=";
    music_id += QString("%1").arg(object_song["id"].toInt());
    QString music_url=music_id;
    music_url += ".mp3";
    qDebug()<<music_url;
    playlist->addMedia(QMediaContent(QUrl(music_url)));
}
//添加音乐
void MP157::addsongs()
{
    QStringList songslist = QFileDialog::getOpenFileNames(this,"选择文件",QDir::homePath(),"*.mp3");
    for(int i=0;i<songslist.size();i++){
        QString song=songslist.at(i);
        //裁剪歌曲名称
        this->playlist->addMedia(QUrl::fromLocalFile(song));
        QStringList songL=song.split('/');
        song=songL.last();
        ui->listWidget_songs->addItem(song);
    }
}
//清除歌曲
void MP157::clearList()
{
    this->playlist->clear();
    ui->listWidget_songs->clear();
}
//删除歌曲
void MP157::deleteList()
{
    int index = ui->listWidget_songs->currentRow();
    if(-1!=index){
        ui->listWidget_songs->takeItem(index);
        this->playlist->removeMedia(index);
    }
}
//播放音乐
void MP157::playPause()
{
    if(m_bStart==true){
        qDebug()<<"m_ptimer start";
        m_ptimer->start(100);
        m_bStart=false;
    }else if(m_bStart==false){
        m_ptimer->stop();
        m_bStart=true;
    }

    ui->horizontalSlider_time->setEnabled(true);
    if(" "==ui->toolButton_play->text()){//通过空格和换行确定播放还是暂停
        int index=ui->listWidget_songs->currentRow();
        if(-1!=index){
            QString songname =ui->listWidget_songs->item(index)->text().left(ui->listWidget_songs->item(index)->text().lastIndexOf("."));
            playlist->setCurrentIndex(index);
            player->play();
            ui->toolButton_play->setText("\n");
            ui->toolButton_play->setStyleSheet("background-image: url(:/new/prefix1/res/音乐播放器_05.png);");
        }
    }else {
        player->pause();
        ui->toolButton_play->setText(" ");
        ui->toolButton_play->setStyleSheet("background-image: url(:/new/prefix1/res/音乐播放器_06.png);");
    }
}
//下一曲
void MP157::playNext()
{
    player->stop();
    int index=playlist->currentIndex();
    if(index==playlist->mediaCount()-1){
        index=0;
    }else{
        index++;
    }
    playlist->setCurrentIndex(index);
    ui->listWidget_songs->setCurrentRow(index);
    ui->toolButton_play->setText(" ");
    ui->toolButton_play->setStyleSheet("background-image: url(:/new/prefix1/res/音乐播放器_06.png);");
}
//上一曲
void MP157::playPre()
{
    player->stop();
    int index=playlist->currentIndex();
    if(0==index){
        index=playlist->mediaCount()-1;
    }else {
        index--;
    }
    playlist->setCurrentIndex(index);
    ui->listWidget_songs->setCurrentRow(index);
    ui->toolButton_play->setText(" ");
    ui->toolButton_play->setStyleSheet("background-image: url(:/new/prefix1/res/音乐播放器_06.png);");
}
//声音
void MP157::setVol(int vol)
{
    player->setVolume(vol);
}

void MP157::set_ID(QString setID)
{
    qDebug()<<setID;
    ui->widget_F4->set_ID(setID);
}
//*******播放图片旋转
//更新旋转图片角度
void MP157::upDateRote()
{
    this->angle +=1;
    if(angle==360){
        qDebug()<<"m_Rote";
        angle=0;
    }
    update();
}
//计算时间
void MP157::durChanged(qint64 dur)
{
    ui->horizontalSlider_time->setRange(0,dur);
    totalTime.clear();
    dur/=1000;
    totalTime+=QString::number(dur/60);
    totalTime+=':';
    if(dur%60<10)
        totalTime+='0';
    totalTime+=QString::number(dur%60);
    ui->label_2->setText(totalTime);
}
//歌曲改变更换时长
void MP157::posChange(qint64 pos)
{
    presentTime.clear();
    ui->horizontalSlider_time->setValue(pos);
    pos/=1000;
    presentTime+=QString::number(pos/60);
    presentTime+=':';
    if(pos%60<10)
        presentTime+='0';
    presentTime+=QString::number(pos%60);
    ui->label->setText(presentTime);
    if(presentTime==totalTime)
    {
        presentTime.clear();
        player->stop();
    }
}

//*****************旋转图片绘画事件bug中
void MP157::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    // 创建绘图设备
    QPixmap pixmap(ui->label__play_image->size());
    pixmap.fill(Qt::transparent);
    // 创建画家对象，并设置绘图设备
    QPainter painter(&pixmap);
    // 设置画笔为无边框
    painter.setPen(Qt::NoPen);
    // 设置渲染提示为反锯齿，使绘制效果更平滑
    painter.setRenderHint(QPainter::Antialiasing, true);
    // 获取绘图区域的中心点坐标
    QPoint centerPoint = ui->label__play_image->rect().center();
    // 平移坐标系到中心点
    painter.translate(centerPoint);
    // 计算半径
    int radius = (std::min(ui->label__play_image->width(), ui->label__play_image->height()) - 10 * 2) / 2;
    // 创建圆形路径
    QPainterPath path;
    path.addEllipse(-radius, -radius, radius*2, radius*2);
    // 将绘制区域限定为圆形
    painter.setClipPath(path);
    // 创建画笔对象，并设置颜色和宽度
    QPen pen;
    pen.setColor(Qt::red);
    pen.setWidth(5);
    // 设置画笔
    painter.setPen(pen);
    // 绘制圆形
    painter.drawEllipse(QPoint(0, 0), radius, radius);
    // 创建矩形区域
    QRect rect = QRect(-radius, -radius, radius*2, radius*2);
    // 设置画笔颜色为蓝色
    pen.setColor(Qt::blue);
    painter.setPen(pen);
    // 绘制弧线
    painter.drawArc(rect.adjusted(-3,-3,3,3), 0, static_cast<int>(angle * 16));
    // 旋转画布
    painter.rotate(angle);
    // 绘制头像图片
    painter.drawPixmap(rect, QPixmap(":/new/prefix1/res/头像.jpg"));
    // 恢复绘制区域限制
    painter.setClipping(false);
    // 将绘制结果设置为 QLabel 的背景图片
    ui->label__play_image->setPixmap(pixmap);
}
//-----------------软键盘事件触发并显示-------------------
bool MP157::eventFilter(QObject *watched, QEvent *event)
{
    //输入捕获判断控件对象
    if(watched == ui->lineEdit_serch && event->type() == QEvent::MouseButtonPress){
        lineEdit = ui->lineEdit_serch;
        callKeyBoard();
    }
    return QWidget::eventFilter(watched,event);
}

void MP157::callKeyBoard()
{
    //调用开源软键盘类并显示
    KeyBoard *keyBoard = new KeyBoard(0,this->lineEdit);
    keyBoard->show();
    //显示位置
    keyBoard->move(0+(this->width() - keyBoard->width())/2,this->height() - keyBoard->height()-20);
}

MP157::~MP157()
{
    delete ui;
}


