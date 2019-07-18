#ifndef FINDPASSWORD_H
#define FINDPASSWORD_H

#include <QWidget>
#include "include/storage.h"

typedef struct FINDPASSWORDINFO
{
    QString emil;
    QString tel;
    QString password;
    QString passwordSURE;
}findpasswordinfo;

namespace Ui {
class FindPassword;
}

class FindPassword : public QWidget
{
    Q_OBJECT

public:
    explicit FindPassword(QWidget *parent = 0);
    ~FindPassword();
    void GetInfo();

private slots:
    void on_pushButton_sure_clicked();

    void on_pushButton_modify_clicked();

private:
    Ui::FindPassword *ui;
    findpasswordinfo info;
    bool ISINFOTRUE = false;
    Storage *mysql;
};

#endif // FINDPASSWORD_H
