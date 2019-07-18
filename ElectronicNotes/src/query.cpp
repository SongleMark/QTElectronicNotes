#include "include/query.h"
#include "ui_query.h"
#include <QMessageBox>

Query::Query(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Query)
{
    ui->setupUi(this);
    move(500,250);
    add = new AddNotes();

    connect(ui->pushButton_add,SIGNAL(clicked(bool)),this,SLOT(PushButtonAddClicked()));
    connect(ui->comboBox,SIGNAL(activated(int)),this,SLOT(ComboBoxActivated(int)));
    connect(ui->pushButton_update,SIGNAL(clicked(bool)),this,SLOT(PushButtonUpdateClicked()));
    connect(ui->tableWidget,SIGNAL(cellClicked(int,int)),this,SLOT(TableWidgetCellClicked(int,int)));
    connect(ui->pushButton_pre,SIGNAL(clicked(bool)),this,SLOT(PushButtonPreClicked()));
    connect(ui->pushButton_next,SIGNAL(clicked(bool)),this,SLOT(PushButtonNextClicked()));
    connect(ui->pushButton_query,SIGNAL(clicked(bool)),this,SLOT(PushButtonQueryClicked()));

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
void Query::PushButtonAddClicked()
{
    add->show();
}

//点击不同的笔记本combox时在tablewidget列出笔记标题
void Query::ComboBoxActivated(int index)
{
    ui->textEdit_show->clear();
    mysql->ReadNotesFromMysql(booklist[index]);
    num_book = mysql->GetNumOfNote(booklist[index]);

    for(int i = 0 ; i<num_book ; i++)
    {
        note[i] = mysql->GetNotes(i);
    }


    ui->tableWidget->setRowCount(num_book);
    ui->tableWidget->setColumnCount(2);

    QStringList header;
    header << QStringLiteral("笔记标题") << QStringLiteral("建立时间");
    ui->tableWidget->setHorizontalHeaderLabels(header);

    //设置tablewidget的行自动填充
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    //将数据显示在tablewidget上
    int row = 0;
    for( row = 0 ; row<num_book ; row++ )
    {
        ui->tableWidget->setItem(row , 0 , new QTableWidgetItem(note[row].title));
        ui->tableWidget->setItem(row , 1 , new QTableWidgetItem(note[row].time));
    }
}

//点击更新按钮
void Query::PushButtonUpdateClicked()
{
    SetStartcombox();
    ui->textEdit_show->clear();
    mysql->ReadNotesFromMysql(booklist[0]);
    num_book = mysql->GetNumOfNote(booklist[0]);

    if(booklist[0].isEmpty())
    {
        return ;
    }

    for(int i = 0 ; i<num_book ; i++)
    {
        note[i] = mysql->GetNotes(i);
    }


    ui->tableWidget->setRowCount(num_book);
    ui->tableWidget->setColumnCount(2);

    QStringList header;
    header << QStringLiteral("笔记标题") << QStringLiteral("建立时间");
    ui->tableWidget->setHorizontalHeaderLabels(header);

    //设置tablewidget的行自动填充
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    //将数据显示在tablewidget上
    int row = 0;
    for( row = 0 ; row<num_book ; row++ )
    {
        ui->tableWidget->setItem(row , 0 , new QTableWidgetItem(note[row].title));
        ui->tableWidget->setItem(row , 1 , new QTableWidgetItem(note[row].time));
    }
}

//点击笔记列表会在右侧显示笔记内容
void Query::TableWidgetCellClicked(int row, int column)
{
    if(column == 1)
    {
        ui->textEdit_show->clear();
        QMessageBox::information(NULL,"请点击左侧内容","点击错误",
                                 QMessageBox::Yes|QMessageBox::No,QMessageBox::Yes);
        return ;
    }
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
void Query::PushButtonPreClicked()
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
void Query::PushButtonNextClicked()
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
void Query::PushButtonQueryClicked()
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
