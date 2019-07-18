#include "include/query.h"
#include "ui_query.h"
#include <QMessageBox>

Query::Query(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Query)
{
    ui->setupUi(this);
    add = new AddNotes();
}

Query::~Query()
{
    delete ui;
}

//获取登录昵称并显示初始化笔记本
void Query::GetUserFromMainwindow(QString user)
{
    this->user = user;
    add->GetUserFromQuery(user);
    QString welcome = "欢迎" + user + "登录!!!";
    setWindowTitle(welcome);
    SetStartcombox();
}

//初始化combox
void Query::SetStartcombox()
{
    mysql = Storage::getinstance();
    mysql->ConnectMysql();
    mysql->GetAllBook(user);
    num = mysql->GetNumOfBook(user);
    for(int i = 0; i<num ; i++)
    {
        booklist[i] = mysql->Getbooklist(i);
        ui->comboBox->addItem(booklist[i]);
    }

}

//点击添加新笔记按钮
void Query::on_pushButton_add_clicked()
{
    add->show();
}

void Query::on_comboBox_activated(int index)
{
    mysql->ReadNotesFromMysql(booklist[index]);
    num_book = mysql->GetNumOfNote(booklist[index]);

    for(int i = 0 ; i<num_book ; i++)
    {
        note[i] = mysql->GetNotes(i);
    }


    ui->tableWidget->setRowCount(num_book);
    ui->tableWidget->setColumnCount(1);

    QStringList header;
    header << QStringLiteral("笔记标题") ;
    ui->tableWidget->setHorizontalHeaderLabels(header);

    //设置tablewidget的行自动填充
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    //将数据显示在tablewidget上
    int row = 0;
    for( row = 0 ; row<num_book ; row++ )
    {
        ui->tableWidget->setItem(row , 0 , new QTableWidgetItem(note[row].title));
    }
}

//点击笔记列表会在右侧显示笔记内容
void Query::on_tableWidget_cellClicked(int row, int column)
{
    QString title = ui->tableWidget->item(row,column)->text();
    QString content;

    for(int i = 0; i<num_book ; i++)
    {
        if(title == note[i].title)
        {
            content = note[i].content;
            ui->textEdit_show->setText(content);
            break;
        }
        else {
            continue;
        }

    }

    pushbutton_column = row;
    pushbutton_row = column;

}

//点击上一张按钮
void Query::on_pushButton_pre_clicked()
{
    if(ui->tableWidget->item(pushbutton_row,pushbutton_column) == NULL)
    {
        QMessageBox::information(NULL,"没有笔记","请点击笔记本或添加笔记",
                                 QMessageBox::Yes|QMessageBox::No , QMessageBox::Yes);
        return ;

    }
    if(pushbutton_row != 0)
    {
        pushbutton_row --;
        pushbutton_column = 0;
    }
    else {
        pushbutton_row = 0;
        pushbutton_column = 0;
        QMessageBox::information(NULL,"第一张","往上没有了",
                                 QMessageBox::Yes|QMessageBox::No , QMessageBox::Yes);
    }

    QString title = ui->tableWidget->item(pushbutton_row,pushbutton_column)->text();
    QString content;

    for(int i = 0; i<num_book ; i++)
    {
        if(title == note[i].title)
        {
            content = note[i].content;
            ui->textEdit_show->setText(content);
            break;
        }
        else {
            continue;
        }

    }

}

//点击下一张按钮
void Query::on_pushButton_next_clicked()
{
    if(ui->tableWidget->item(pushbutton_row,pushbutton_column) == NULL)
    {
        QMessageBox::information(NULL,"没有笔记","请点击笔记本或添加笔记",
                                 QMessageBox::Yes|QMessageBox::No , QMessageBox::Yes);
        return ;

    }
    if(pushbutton_row != num_book-1 )
    {
        pushbutton_row ++;
        pushbutton_column = 0;
    }
    else {
        pushbutton_row = num_book-1;
        pushbutton_column = 0;
        QMessageBox::information(NULL,"最后一张","往下没有了",
                                 QMessageBox::Yes|QMessageBox::No , QMessageBox::Yes);
    }

    QString title = ui->tableWidget->item(pushbutton_row,pushbutton_column)->text();
    QString content;

    for(int i = 0; i<num_book ; i++)
    {
        if(title == note[i].title)
        {
            content = note[i].content;
            ui->textEdit_show->setText(content);
            break;
        }
        else {
            continue;
        }

    }

}

//点击搜索按钮
void Query::on_pushButton_query_clicked()
{
    QString title = ui->lineEdit_query->text();
    QString content = mysql->GetQueryInfo(title,user);
    if(content.isEmpty())
    {
        ui->textEdit_show->setText("笔记不存在，请重新搜索");
    }
    else {
        ui->textEdit_show->setText(content);
    }
}
