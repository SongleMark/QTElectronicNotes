#ifndef QUERY_H
#define QUERY_H

#include <QWidget>
#include "include/addnotes.h"

namespace Ui {
class Query;
}

class Query : public QWidget
{
    Q_OBJECT

public:
    explicit Query(QWidget *parent = 0);
    ~Query();
    void GetUserFromMainwindow(QString user);//获取登录昵称
    void SetStartcombox();

private slots:
    void on_pushButton_add_clicked();

    void on_comboBox_activated(int index);

    void on_tableWidget_cellClicked(int row, int column);

    void on_pushButton_pre_clicked();

    void on_pushButton_next_clicked();

    void on_pushButton_query_clicked();

private:
    Ui::Query *ui;
    QString user;
    AddNotes *add;
    Storage *mysql;
    QString booklist[NUM];
    int num;
    NOTES note[NUM];
    int num_book;

    int pushbutton_row;
    int pushbutton_column;
};

#endif // QUERY_H
