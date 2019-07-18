#include "include/findpassword.h"
#include "ui_findpassword.h"
#include <QMessageBox>

FindPassword::FindPassword(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FindPassword)
{
    ui->setupUi(this);
    move(500,250);
    ui->lineEdit_password->setEnabled(false);
    ui->lineEdit_passwordsure->setEnabled(false);

}

FindPassword::~FindPassword()
{
    delete ui;
}

void FindPassword::GetInfo()
{
    if(ui->lineEdit_tel->text() == "" || ui->lineEdit_tel->text() == "")
    {
        ui->lineEdit_tel->setPlaceholderText(QStringLiteral("请输入电话"));
        ui->lineEdit_emil->setPlaceholderText(QStringLiteral("请输入emil"));
        return ;
    }

    mysql = Storage::getinstance();
    mysql->ConnectMysql();
    ui->lineEdit_password->setEnabled(false);
    ui->lineEdit_passwordsure->setEnabled(false);
    info.tel = ui->lineEdit_tel->text();
    QString emil = mysql->GetEmil(info.tel);
    info.emil = ui->lineEdit_emil->text();
    if(emil == info.emil)
    {
        ISINFOTRUE = true;
    }
    if(ISINFOTRUE)
    {
        ui->label_return->setText("请输入密码");
        ui->lineEdit_password->setEnabled(true);
        ui->lineEdit_passwordsure->setEnabled(true);
        ui->lineEdit_password->setEchoMode(QLineEdit::Password);
        ui->lineEdit_passwordsure->setEchoMode(QLineEdit::Password);

    }
    else
    {
        ui->label_return->setText("输入信息错误");
        QMessageBox::information(NULL,"输入信息错误","请重新输入",QMessageBox::Yes|QMessageBox::No,
                                 QMessageBox::Yes);
        ui->lineEdit_tel->clear();
        ui->lineEdit_emil->clear();
        return ;
    }

}

//点击输入确认
void FindPassword::on_pushButton_sure_clicked()
{
    GetInfo();

}

//点击更改
void FindPassword::on_pushButton_modify_clicked()
{
    if(ui->lineEdit_password->text() == "")
    {
        QMessageBox::information(NULL,"密码不能为空","密码不能为空",QMessageBox::Yes|QMessageBox::No,
                                 QMessageBox::Yes);
        return ;
    }
    if(ISINFOTRUE)
    {
        info.password = ui->lineEdit_password->text();
        info.passwordSURE = ui->lineEdit_passwordsure->text();
        if(info.password == info.passwordSURE)
        {
            mysql->ModifyPassword(info.tel,info.password);
            QMessageBox::information(NULL,"成功","密码修改成功",QMessageBox::Yes|QMessageBox::No,
                                     QMessageBox::Yes);
            return ;
        }
        else
        {
            ui->lineEdit_passwordsure->clear();
            ui->lineEdit_passwordsure->setPlaceholderText(QStringLiteral("两次密码不一致"));
            ui->lineEdit_password->clear();
            return ;
        }
    }

}
