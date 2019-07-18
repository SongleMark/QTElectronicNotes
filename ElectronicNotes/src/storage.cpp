#include "include/storage.h"
#include <QMessageBox>
#include <QDebug>
#include <QDateTime>
#define TABLE  "userinfo"
#define TABLENOTES "books"

Storage *Storage::instance = nullptr;

Storage *Storage::getinstance()
{
    if(instance == nullptr)
    {
        instance = new Storage();
    }

    return instance;
}

Storage::Storage()
{

}

Storage::~Storage()
{
    delete instance;
}

//连接数据库
bool Storage::ConnectMysql()
{
    mysql_info.db = QSqlDatabase::addDatabase("QMYSQL");
    mysql_info.db.setDatabaseName("ElectronicNotebook");
    mysql_info.db.setUserName("root");
    mysql_info.db.setHostName("localhost");
    mysql_info.db.setPort(3306);
    mysql_info.db.setPassword("123456");
    if(!mysql_info.db.open())
    {
        QMessageBox::information(NULL,"数据库打开失败","无法打开数据库",
                                 QMessageBox::Yes|QMessageBox::No , QMessageBox::Yes);
        return false;
    }

    mysql_info.query = new QSqlQuery(mysql_info.db);

    return true ;
}

//向数据库插入数值
void Storage::AddValueToMysql(USER_INFO user_info)
{
    QStringList table = mysql_info.db.tables();
    if(!table.contains(TABLE))
    {
        QString sql = QString("CREATE TABLE %1 (name varchar(32) ,"
                              "                 password varchar(32),"
                              "                 sex char(12),"
                              "                 tel varchar(32),"
                              "                 emil varchar(32),"
                              "                 time varchar(32));").arg(TABLE);
        qDebug() << "sql = " << sql ;
        mysql_info.query->exec(sql);
    }

    QString sql_check = QString("select count(*) from %1 where name='%2';")
                               .arg(TABLE)
                               .arg(user_info.name);
    qDebug() << "sql_check = " << sql_check;
    mysql_info.query->exec(sql_check);

    if(mysql_info.query->next())
    {
        int count = mysql_info.query->value(0).toInt();
        if(count >= 1)
        {
            QMessageBox::information(NULL,"该用户名已存在","请重新注册用户名",
                                     QMessageBox::Yes|QMessageBox::No,QMessageBox::Yes);
            return;
        }
    }

    QString sql_check_tel = QString("select count(*) from %1 where tel='%2';")
                               .arg(TABLE)
                               .arg(user_info.tel);
    qDebug() << "sql_check_tel = " << sql_check_tel;
    mysql_info.query->exec(sql_check_tel);

    if(mysql_info.query->next())
    {
        int count = mysql_info.query->value(0).toInt();
        if(count >= 1)
        {
            QMessageBox::information(NULL,"该号码已注册过","请重新注册号码",
                                     QMessageBox::Yes|QMessageBox::No,QMessageBox::Yes);
            return;
        }
    }

    QString sql_check_emil = QString("select count(*) from %1 where emil='%2';")
                               .arg(TABLE)
                               .arg(user_info.emil);
    qDebug() << "sql_check_emil = " << sql_check_emil;
    mysql_info.query->exec(sql_check_emil);

    if(mysql_info.query->next())
    {
        int count = mysql_info.query->value(0).toInt();
        if(count >= 1)
        {
            QMessageBox::information(NULL,"该emil已注册过","请重新注册emil",
                                     QMessageBox::Yes|QMessageBox::No,QMessageBox::Yes);
            return;
        }
    }
    QDateTime current_time = QDateTime::currentDateTime();
    QString time = current_time.toString("yyyy年MM月dd日hh:mm:ss");
    QString sql = QString("insert into %1 values('%2' , '%3' , '%4' , '%5' , '%6' , '%7');")
                                                .arg(TABLE)
                                                .arg(user_info.name)
                                                .arg(user_info.password)
                                                .arg(user_info.sex)
                                                .arg(user_info.tel)
                                                .arg(user_info.emil)
                                                .arg(time);
    qDebug() << "time = " << time;
    qDebug() << "sql = " << sql;
    mysql_info.query->exec(sql);

}

//从数据库读取数值
void Storage::ReadValueFromMysql(USER_INFO &user_info)
{
    QString sql = QString("select * from %1 ;").arg(TABLE);
    qDebug() << "sql = " << sql;

    mysql_info.query->exec(sql);
    if(mysql_info.query->next())
    {
        user_info.name = mysql_info.query->value(0).toString();
        user_info.password = mysql_info.query->value(1).toString();
        user_info.sex = mysql_info.query->value(2).toString();
        user_info.tel = mysql_info.query->value(3).toString();
        user_info.emil = mysql_info.query->value(4).toString();
        user_info.time = mysql_info.query->value(5).toString();
    }
    qDebug() << "name = " << user_info.name << " password = " << user_info.password
             << "sex = " << user_info.sex << "tel = " << user_info.tel
             << "emil = " << user_info.emil << "time = " << user_info.time;
}

//从数据库获取数据数目
int Storage::GetCountFromMysql()
{
    QString sql = QString("select count(*) from %1 ; ").arg(TABLE);
    qDebug() << "sql = " << sql;

    mysql_info.query->exec(sql);
    int count;
    if(mysql_info.query->next())
    {
        count = mysql_info.query->value(0).toInt();
        qDebug() << "count = " << count;
    }

    return count ;

}

//根据用户名从数据库获取密码
QString Storage::GetPasswordFromMysql(QString name)
{
    QString password;
    QString sql = QString("select password from %1 where tel = '%2'")
                         .arg(TABLE)
                         .arg(name);
    qDebug() << "sql = " << sql;
    mysql_info.query->exec(sql);
    if(mysql_info.query->next())
    {
        password = mysql_info.query->value(0).toString();
        qDebug() << "password = " << password;
    }
    if(password.isEmpty())
    {
        sql = QString("select password from %1 where emil = '%2'")
                     .arg(TABLE)
                     .arg(name);
        mysql_info.query->exec(sql);
        if(mysql_info.query->next())
        {
            password = mysql_info.query->value(0).toString();
            qDebug() << "password = " << password;
        }
    }

    return password;
}

//根据电话或emil获取用户名
QString Storage::GetuserFromMysql(QString name)
{
    QString user;
    QString sql = QString("select name from %1 where tel = '%2'")
                         .arg(TABLE)
                         .arg(name);
    qDebug() << "sql = " << sql;
    mysql_info.query->exec(sql);
    if(mysql_info.query->next())
    {
        user = mysql_info.query->value(0).toString();
        qDebug() << "user = " << user;
    }
    if(user.isEmpty())
    {
        sql = QString("select name from %1 where emil = '%2'")
                     .arg(TABLE)
                     .arg(name);
        mysql_info.query->exec(sql);
        if(mysql_info.query->next())
        {
            user = mysql_info.query->value(0).toString();
            qDebug() << "password = " << user;
        }
    }

    return user;

}

//插入笔记信息
void Storage::AddNotesToMysql(NOTES note,QString tables)
{
    QStringList table = mysql_info.db.tables();
    if(!table.contains(tables))
    {
        QString sql = QString("CREATE TABLE %1 (notetitle varchar(32),notecontent varchar(2000),time varchar(32));")
                             .arg(tables);
        qDebug() << "sql = " << sql;
        mysql_info.query->exec(sql);
    }

    QString sql = QString("insert into %1 values('%2','%3','%4');")
                         .arg(tables)
                         .arg(note.title)
                         .arg(note.content)
                         .arg(note.time);
    qDebug() << "sql = " << sql;
    mysql_info.query->exec(sql);

}

//建立表名为登录人名，并且将笔记本名存入
void Storage::AddBooksFromMysql(QString book,QString user)
{
    QStringList table = mysql_info.db.tables();
    if(!table.contains(user))
    {
        QString sql = QString("CREATE TABLE %1 (notebook varchar(32));").arg(user);
        qDebug() << "sql = " << sql;
        mysql_info.query->exec(sql);
    }

    QString sql_check = QString("select count('%1') from %2;").arg(book).arg(user);
    qDebug() << "sql_check = " << sql_check;
    mysql_info.query->exec(sql_check);
    if(mysql_info.query->next())
    {
        int count = mysql_info.query->value(0).toInt();
        if(count >= 1)
        {
            return ;
        }
    }

    QString sql = QString("insert into %1 values('%2');")
                         .arg(user)
                         .arg(book);
    qDebug() << "sql = " << sql;
    mysql_info.query->exec(sql);
}

//用户获取已经建立的笔记本
void Storage::GetAllBook(QString user)
{
    QString sql =QString("select * from %1;").arg(user);
    qDebug() << "sql = " << sql;
    mysql_info.query->exec(sql);
    int i = 0;
    while(mysql_info.query->next())
    {
        booklist[i] = mysql_info.query->value(0).toString();
        qDebug() << "booklist[%1] = %2" << i,booklist[i];
        i++;
    }
}

//获取某个用户创建的笔记本数量
int Storage::GetNumOfBook(QString user)
{
    int count;
    QString sql =QString("select count(*) from %1;").arg(user);
    qDebug() << "sql = " << sql;
    mysql_info.query->exec(sql);
    if(mysql_info.query->next())
    {
        count = mysql_info.query->value(0).toInt();
        qDebug() << "count = " << count;
    }

    return count;

}

//获取笔记标题以及内容
void Storage::ReadNotesFromMysql(QString user)
{
    QString sql = QString("select * from %1;").arg(user);
    qDebug() << "sql = " << sql;

    mysql_info.query->exec(sql);
    int i = 0;
    while(mysql_info.query->next())
    {
        note[i].title = mysql_info.query->value(0).toString();
        note[i].content = mysql_info.query->value(1).toString();
        note[i].time = mysql_info.query->value(2).toString();
        //qDebug() << "note[%1] = " << i << note[i].title;
        i++;
    }

}

//向外部传输笔记本名
QString Storage::Getbooklist(int i)
{
    return booklist[i];
}

//向外部传输笔记标题内容和建立时间
NOTES Storage::GetNotes(int i)
{
    return note[i];
}

//获取某个用户的某个笔记本内容的条数
int Storage::GetNumOfNote(QString book)
{
    int count;
    QString sql =QString("select count(*) from %1;").arg(book);
    qDebug() << "sql = " << sql;
    mysql_info.query->exec(sql);
    if(mysql_info.query->next())
    {
        count = mysql_info.query->value(0).toInt();
        qDebug() << "count = " << count;
    }

    return count;

}

//用户根据笔记标题搜索内容
QString Storage::GetQueryInfo(QString note,QString user)
{
    QString content;
    GetAllBook(user);//先获用户已经创建的笔记本
    int num = GetNumOfBook(user);//获取笔记本数量
    for( int i = 0 ; i<num ; i++ )
    {
        QString sql = QString("select notecontent from %1 where notetitle='%2';").arg(booklist[i]).arg(note);
        qDebug() << "sql = " << sql;
        mysql_info.query->exec(sql);
        if(mysql_info.query->next())
        {
            content = mysql_info.query->value(0).toString();
            qDebug() << "content = " << content;
            if(!content.isEmpty())
            {
                return content;
            }
            else
            {
                continue;
            }
        }

    }

}

//重置密码时先根据电话查找emil
QString Storage::GetEmil(QString tel)
{
    QString sql = QString("select emil from %1 where tel='%2';").arg(TABLE).arg(tel);
    qDebug() << "sql = " << sql;
    QString emil;
    mysql_info.query->exec(sql);
    if(mysql_info.query->next())
    {
        emil = mysql_info.query->value(0).toString();
        qDebug() << "emil = " << emil;
    }

    return emil;
}

//修改密码
void Storage::ModifyPassword(QString tel,QString password)
{
    QString sql = QString("update %1 set password='%2' where tel='%3';")
                         .arg(TABLE)
                         .arg(password)
                         .arg(tel);
    qDebug() << "sql = " << sql;
    mysql_info.query->exec(sql);
}

