#ifndef REGISTER_H
#define REGISTER_H

#include <QWidget>
#include "include/storage.h"

namespace Ui {
class Register;
}

class Register : public QWidget
{
    Q_OBJECT

public:
    explicit Register(QWidget *parent = 0);
    ~Register();
    void GetRegisterInfo();
    void ClearRegisterInfo();

private slots:
    void RegisterClicked();

private:
    Ui::Register *ui;
    USER_INFO userinfo;
    Storage *mysql;
};

#endif // REGISTER_H
