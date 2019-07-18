#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include "include/register.h"
#include "include/storage.h"
#include "include/query.h"
#include "include/findpassword.h"

namespace Ui {
class Mainwindow;
}

class Mainwindow : public QWidget
{
    Q_OBJECT

public:
    explicit Mainwindow(QWidget *parent = 0);
    ~Mainwindow();
    bool LoginNote();
    bool eventFilter(QObject *watched, QEvent *event);

private slots:
    void RegisterClicked();
    void LoginClicked();

private:
    Ui::Mainwindow *ui;
    Register *register_r;
    Storage *mysql;
    QString name;
    Query *query;
    FindPassword *find;
};

#endif // MAINWINDOW_H
