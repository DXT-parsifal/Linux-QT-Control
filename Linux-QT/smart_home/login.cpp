#include "login.h"
#include "ui_login.h"
#include "regist.h"
#include <QMessageBox>
//软键盘
#include "keyboard.h"

Login::Login(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Login)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/new/prefix1/res/智能家居3.png"));
    this->setWindowTitle("Smart Campus");
    TcpSocket = new QTcpSocket(this);
    TcpSocket->connectToHost("139.199.212.89",10000);

    connect(TcpSocket,SIGNAL(readyRead()),this,SLOT(slot_TcpSocket_readyRead()));

    //------------软键盘事件开启捕获-------------------
    ui->lineEdit_ID->installEventFilter(this);
    ui->lineEdit_Pass->installEventFilter(this);

}

void Login::slot_TcpSocket_readyRead()
{
    /**** 接收TCP服务器数据 ****/
    QByteArray data = TcpSocket->readAll();
    /**** 解析JSON格式 *********/
    QJsonDocument JsonDocument = QJsonDocument::fromJson(data);
    if(JsonDocument.isObject() != true) return;
    /**** 转为JSON对象 *********/
    QJsonObject json =JsonDocument.object();
    /**** 处理 *****************/
    if(json["CMD"].toString() == "登录结果")
    {
        if(json["State"].toBool() == true)
        {
            QString ID = ui->lineEdit_ID->text();
            this->mp157 = new MP157();
            mp157->set_ID(ID);
            mp157->show();
            this->close();  //关闭登录界面
            TcpSocket->close();
        }
        else
        {
            QMessageBox::warning(this,"登录提示","登录失败,账号密码错误");
        }
    }
}

Login::~Login()
{
    delete lineEdit;
    delete mp157;
    delete ui;
}


void Login::on_pushButton_login_clicked()
{
    QString ID = ui->lineEdit_ID->text();
    QString pass = ui->lineEdit_Pass->text();
    if(ID.isEmpty() || pass.isEmpty() ) return ;    //不能让其为空

    QJsonObject json;
    json["CMD"] = "登录";
    json["ID"] = ID;
    json["pass"] = pass;

    /*---- 发送 ----*/
    QByteArray data = QJsonDocument(json).toJson();

    TcpSocket->write(data);
}

void Login::on_pushButton_regiset_clicked()
{
    regist *reg=new regist;
    this->close();//登录界面关闭
    reg->show();//打开注册界面
    TcpSocket->close();//关闭TCP通信
}

//-----------------软键盘事件触发并显示-------------------
bool Login::eventFilter(QObject *watched, QEvent *event)
{
    //输入捕获判断控件对象
    if(watched == ui->lineEdit_ID && event->type() == QEvent::MouseButtonPress){
        lineEdit = ui->lineEdit_ID;
        callKeyBoard();
    }else if(watched == ui->lineEdit_Pass && event->type() == QEvent::MouseButtonPress){
        lineEdit = ui->lineEdit_Pass;
        callKeyBoard();
    }
    return QWidget::eventFilter(watched,event);
}

void Login::callKeyBoard()
{
    //调用开源软键盘类并显示
    KeyBoard *keyBoard = new KeyBoard(0,this->lineEdit);
    keyBoard->show();
    //显示位置
    keyBoard->move(0+(this->width() - keyBoard->width())/2,this->height() - keyBoard->height());
}
