#include "include/mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QDebug>

Mainwindow::Mainwindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Mainwindow)
{
    ui->setupUi(this);
    setWindowTitle("Electronic Notes");
    ui->user->setPlaceholderText(QStringLiteral("请输入邮箱或电话号码"));
    ui->password->setPlaceholderText(QStringLiteral("请输入密码"));
    ui->password->setEchoMode(QLineEdit::Password);
    mysql = Storage::getinstance();


    connect(ui->register_2,SIGNAL(clicked(bool)),this,SLOT(RegisterClicked()));
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

bool Mainwindow::LoginDictionary()
{
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

void Mainwindow::on_login_clicked()
{
    bool falg = LoginDictionary();
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
        QMessageBox::information(NULL,"密码输入错误","请重新输入",QMessageBox::Yes|QMessageBox::No,QMessageBox::Yes);
        ui->password->clear();
        ui->user->clear();
    }
}
