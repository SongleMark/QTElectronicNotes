#include "include/mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QDebug>
#include <QMouseEvent>

Mainwindow::Mainwindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Mainwindow)
{
    ui->setupUi(this);
    move(500,250);
    setWindowTitle("Electronic Notes");
    ui->forgetp->setText("<u>忘记密码</u>");//设置字体有下划线
    ui->user->setPlaceholderText(QStringLiteral("请输入邮箱或电话号码"));
    ui->password->setPlaceholderText(QStringLiteral("请输入密码"));
    ui->password->setEchoMode(QLineEdit::Password);
    ui->forgetp->installEventFilter(this);//为该lable安装时间过滤器
    mysql = Storage::getinstance();


    connect(ui->register_2,SIGNAL(clicked(bool)),this,SLOT(RegisterClicked()));
    connect(ui->login,SIGNAL(clicked(bool)),this,SLOT(LoginClicked()));
}

Mainwindow::~Mainwindow()
{
    delete ui;
}

//点击主界面注册按钮
void Mainwindow::RegisterClicked()
{
    register_r = new Register();
    register_r->show();
}

//判断登录密码数否正确
bool Mainwindow::LoginNote()
{
    if(ui->password->text() == "")
    {
        QMessageBox::information(NULL,"密码为空","密码不能为空",QMessageBox::Yes|QMessageBox::No,QMessageBox::Yes);
        return false;
    }
    name = ui->user->text();
    mysql->ConnectMysql();
    QString password = mysql->GetPasswordFromMysql(name);
    if(password == ui->password->text())
    {
        return true;
    }
    else
    {
        return false;
    }

}

//点击登录按钮槽函数
void Mainwindow::LoginClicked()
{
    bool falg = LoginNote();
    qDebug() << "flag = " << falg;

    if(falg)
    {
        QString user = mysql->GetuserFromMysql(name);
        query = new Query();
        query->GetUserFromMainwindow(user);
        query->show();
        qDebug() << "登录成功" ;
    }
    else
    {
        QMessageBox::information(NULL,"输入错误","请重新输入密码或用户名",QMessageBox::Yes|QMessageBox::No,QMessageBox::Yes);
        ui->password->clear();
        ui->user->clear();
        return ;
    }
}


bool Mainwindow::eventFilter(QObject *watched, QEvent *event)
{
    if(watched == ui->forgetp)
    {
        if(event->type() == QEvent::MouseButtonPress)
        {
            QMouseEvent *mouseevent = static_cast<QMouseEvent *>(event);
            if(mouseevent->button() == Qt::LeftButton)
            {
                //to do
                find = new FindPassword;
                find->show();
            }
            else
            {
                return false;
            }
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}
