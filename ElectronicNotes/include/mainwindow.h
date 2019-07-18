#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include "include/register.h"
#include "include/storage.h"
#include "include/query.h"

namespace Ui {
class Mainwindow;
}

class Mainwindow : public QWidget
{
    Q_OBJECT

public:
    explicit Mainwindow(QWidget *parent = 0);
    ~Mainwindow();
    bool LoginDictionary();

private slots:
    void RegisterClicked();

    void on_login_clicked();

private:
    Ui::Mainwindow *ui;
    Register *register_r;
    Storage *mysql;
    QString name;
    Query *query;
};

#endif // MAINWINDOW_H
