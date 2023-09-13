#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>
/*---------- TCP相关 -----------*/
#include <QTcpSocket>
/*---------- JSON相关 ----------*/
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include "mp157.h"
#include <QLineEdit>
QT_BEGIN_NAMESPACE
namespace Ui { class Login; }
QT_END_NAMESPACE

class Login : public QWidget
{
    Q_OBJECT

public:
    Login(QWidget *parent = nullptr);
    ~Login();
    //-----------软键盘----------------------
protected:
    bool eventFilter(QObject *watched, QEvent *event);
private:
    QLineEdit *lineEdit;
    void callKeyBoard();

public slots:
    void slot_TcpSocket_readyRead();
private slots:
    void on_pushButton_login_clicked();

    void on_pushButton_regiset_clicked();

private:
    Ui::Login *ui;
    QTcpSocket *TcpSocket;
    MP157 *mp157;

};
#endif // LOGIN_H
