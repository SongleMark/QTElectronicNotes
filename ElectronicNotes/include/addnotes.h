#ifndef ADDNOTES_H
#define ADDNOTES_H

#include <QWidget>
#include "include/storage.h"

enum
{
    CREATE ,
    ORIGINAL
};

namespace Ui {
class AddNotes;
}

class AddNotes : public QWidget
{
    Q_OBJECT

public:
    explicit AddNotes(QWidget *parent = 0);
    ~AddNotes();
    void GetinfoFromInput();
    void GetUserFromQuery(QString user);

private slots:
    void ClearClicked();
    void AddnoteClicked();
    void CreatebookClicked();
    void CriginabookClicked();

private:
    Ui::AddNotes *ui;
    Storage *mysql;
    NOTES note;
    int BookFlag = CREATE;
    QString booktable; //笔记本(在数据库中为表,没一个笔记本一个表)
    QString user;
    QString booklist[NUM];
};

#endif // ADDNOTES_H
