#include "include/register.h"
#include "ui_register.h"
#include <QMessageBox>
#include <QDebug>

Register::Register(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Register)
{
    ui->setupUi(this);
    move(500,250);
    setWindowTitle("Register");
    ui->password->setPlaceholderText(QStringLiteral("密码不得少于6位"));
    ui->password->setEchoMode(QLineEdit::Password);
    ui->password_sure->setEchoMode(QLineEdit::Password);
    mysql = Storage::getinstance();

    connect(ui->register_2,SIGNAL(clicked(bool)),this,SLOT(RegisterClicked()));
}

Register::~Register()
{
    delete ui;
}

//获取注册信息
void Register::GetRegisterInfo()
{
    userinfo.name = ui->name->text();
    userinfo.emil = ui->emil->text();
    userinfo.password = ui->password->text();
    userinfo.sex = ui->sex->currentText();
    userinfo.tel = ui->tel->text();

}

//清除注册控件内容
void Register::ClearRegisterInfo()
{
    ui->name->clear();
    ui->emil->clear();
    ui->password->clear();
    ui->tel->clear();
    ui->password_sure->clear();
}

//点击注册按钮
void Register::RegisterClicked()
{
    GetRegisterInfo();
    if(userinfo.password != ui->password_sure->text() || ui->password_sure->text().length() < 6)
    {
        ui->password_sure->clear();
        ui->password_sure->setPlaceholderText(QStringLiteral("两次密码输入不一致或小于6位"));
        QMessageBox::information(NULL,"没有注册","请重新输入密码",
                                 QMessageBox::Yes|QMessageBox::No,QMessageBox::Yes);
        ui->password->clear();
        return ;
    }
    if(!userinfo.tel.contains(QRegExp("[0-9]")) || userinfo.tel.contains(QRegExp("[a-z]")) || userinfo.tel.contains(QRegExp("[A-Z]")) ||
        userinfo.tel.length() < 7)
    {
        ui->tel->clear();
        ui->tel->setPlaceholderText(QStringLiteral("电话输入格式错误"));
        QMessageBox::information(NULL,"没有注册","请重新输入电话",
                                 QMessageBox::Yes|QMessageBox::No,QMessageBox::Yes);
        return ;
    }
    int choose;
    choose = QMessageBox::question(this,"确认","是否注册?",QMessageBox::Yes|QMessageBox::No);

    if(choose == QMessageBox::Yes)
    {
        mysql->ConnectMysql();
        mysql->AddValueToMysql(userinfo);
        qDebug() << "注册成功";
    }
    else if(choose == QMessageBox::No || userinfo.password.length() < 6)
    {
        QMessageBox::information(NULL,"没有注册","格式不正确或取消注册",
                                 QMessageBox::Yes|QMessageBox::No,QMessageBox::Yes);
        qDebug() << "没有注册";
        return;
    }

    ClearRegisterInfo();
}
