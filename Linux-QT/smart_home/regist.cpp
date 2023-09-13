#include "login.h"
#include "regist.h"
#include "ui_regist.h"

#include <QMessageBox>
#include "keyboard.h"
regist::regist(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::regist)
{
    ui->setupUi(this);

    TcpSocket = new QTcpSocket(this);
    TcpSocket->connectToHost("139.199.212.89",10000);

    connect(TcpSocket,SIGNAL(readyRead()),this,SLOT(slot_TcpSocket_readyRead()));

    //------------软键盘事件开启捕获-------------------
    ui->lineEdit_ID->installEventFilter(this);
    ui->lineEdit_email->installEventFilter(this);
    ui->lineEdit_name->installEventFilter(this);
    ui->lineEdit_pass->installEventFilter(this);

}

regist::~regist()
{
    delete ui;
}

void regist::slot_TcpSocket_readyRead(){
    /**** 接收TCP服务器数据 ****/
    QByteArray data = TcpSocket->readAll();
    /**** 解析JSON格式 *********/
    QJsonDocument JsonDocument = QJsonDocument::fromJson(data);
    if(JsonDocument.isObject() != true) return;
    /**** 转为JSON对象 *********/
    QJsonObject json =JsonDocument.object();
    /**** 处理 *****************/
    if(json["CMD"].toString() == "注册结果")
    {
        if(json["State"].toBool() == true)
        {
            Login *log=new Login();

            this->close();//注册界面关闭
            log->show();//登录界面打开
            TcpSocket->close();
        }
        else
        {
            QMessageBox::warning(this,"注册提示","注册失败,账号密码重复");
        }
    }
}

void regist::on_pushButton_registe_clicked()
{
    QJsonObject json;
    json["CMD"] = "注册";
    json["ID"] = ui->lineEdit_ID->text();
    json["name"]=ui->lineEdit_name->text();
    json["pass"] = ui->lineEdit_pass->text();
    json["email"]=ui->lineEdit_email->text();

    if(json["ID"].isNull()||json["name"].isNull()||json["pass"].isNull()||json["email"].isNull()) return;

    /*---- 发送 ----*/
    QByteArray data = QJsonDocument(json).toJson();

    TcpSocket->write(data);
}

//-----------------软键盘事件触发并显示-------------------
bool regist::eventFilter(QObject *watched, QEvent *event)
{
    //输入捕获判断控件对象
    if(watched == ui->lineEdit_email && event->type() == QEvent::MouseButtonPress){
        lineEdit = ui->lineEdit_email;
        callKeyBoard();
    }else if(watched == ui->lineEdit_ID && event->type() == QEvent::MouseButtonPress){
        lineEdit = ui->lineEdit_ID;
        callKeyBoard();
    }else if(watched == ui->lineEdit_name && event->type() == QEvent::MouseButtonPress){
        lineEdit = ui->lineEdit_name;
        callKeyBoard();
    }else if(watched == ui->lineEdit_pass && event->type() == QEvent::MouseButtonPress){
        lineEdit = ui->lineEdit_pass;
        callKeyBoard();
    }
    return QWidget::eventFilter(watched,event);
}

void regist::callKeyBoard()
{
    //调用开源软键盘类并显示
    KeyBoard *keyBoard = new KeyBoard(0,this->lineEdit);
    keyBoard->show();
    //显示位置
    keyBoard->move(0+(this->width() - keyBoard->width())/2,this->height() - keyBoard->height());
}
