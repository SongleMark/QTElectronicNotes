#include "include/addnotes.h"
#include "ui_addnotes.h"
#include <QMessageBox>
#include <QDateTime>

AddNotes::AddNotes(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AddNotes)
{
    ui->setupUi(this);
    move(500,250);
    mysql = Storage::getinstance();

    connect(ui->clear,SIGNAL(clicked(bool)),this,SLOT(ClearClicked()));
    connect(ui->addnote,SIGNAL(clicked(bool)),this,SLOT(AddnoteClicked()));
    connect(ui->createbook,SIGNAL(clicked(bool)),this,SLOT(CreatebookClicked()));
    connect(ui->originabook,SIGNAL(clicked(bool)),this,SLOT(CriginabookClicked()));
}

AddNotes::~AddNotes()
{
    delete ui;
}

//获取输入的内容
void AddNotes::GetinfoFromInput()
{
    //如果选择新建笔记本
    if(BookFlag == CREATE)
    {
        booktable = ui->notebook->text();
    }//如果选择原有的笔记本
    else if(BookFlag == ORIGINAL)
    {
        booktable = ui->book->currentText();
    }
    note.content = ui->contents->toPlainText();
    note.title = ui->title->text();
    QDateTime current_time = QDateTime::currentDateTime();
    note.time = current_time.toString("yyyy年MM月dd日hh:mm:ss");

}

//从query类获取user
void AddNotes::GetUserFromQuery(QString user)
{
    this->user = user;
    QString show = "欢迎" + user + "登录";
    setWindowTitle(show);

    mysql->ConnectMysql();
    mysql->GetAllBook(user);
    int num = mysql->GetNumOfBook(user);
    for(int i = 0; i<num ; i++)
    {
        booklist[i] = mysql->Getbooklist(i);
        ui->book->addItem(booklist[i]);
    }
}

//点击清除
void AddNotes::ClearClicked()
{
    ui->contents->clear();
}

//点击添加笔记按钮
void AddNotes::AddnoteClicked()
{
    int choose;
    choose = QMessageBox::question(this,"确认","是否添加笔记?",QMessageBox::Yes|QMessageBox::No);

    if(choose == QMessageBox::Yes)
    {
        //获取输入的内容
        GetinfoFromInput();
        mysql->ConnectMysql();
        mysql->AddBooksFromMysql(booktable,user);//向数据库以用户名创建表名并加入笔记本名称
        mysql->AddNotesToMysql(note,booktable);//向数据库加入笔记标题内容建立时间
    }
    else if(choose == QMessageBox::No)
    {
        QMessageBox::information(NULL,"没有添加","你选择不添加",QMessageBox::Yes|QMessageBox::No,QMessageBox::Yes);
        return ;
    }

}

//点击新建笔记本
void AddNotes::CreatebookClicked()
{
    BookFlag = CREATE;
    ui->book->setEnabled(false);
    ui->notebook->setEnabled(true);
}

//点击原有笔记本
void AddNotes::CriginabookClicked()
{
    BookFlag = ORIGINAL;
    ui->book->setEnabled(true);
    ui->notebook->setEnabled(false);
}
