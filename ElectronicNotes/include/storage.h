#ifndef STORAGE_H
#define STORAGE_H
#include <QSqlDatabase>
#include <QSqlQuery>
#define NUM 1000
typedef struct MYSQLINFO
{
    QSqlDatabase db;
    QSqlQuery *query;
}MYSQL_INFO;

//用户信息
typedef struct USERINFO
{
    QString name;    //用户名
    QString password;//密码
    QString sex;     //性别
    QString tel;     //电话
    QString emil;    //邮箱
    QString time;    //注册时间
}USER_INFO;

//笔记信息
typedef struct NOTE
{
    QString title;  //标题
    QString content;//内容
    QString time;   //建立笔记的时间
}NOTES;

class Storage
{
public:
    static Storage *instance;
    static Storage *getinstance();
    bool ConnectMysql();//连接数据库
    void AddValueToMysql(USER_INFO user_info);//插入人员数据
    void ReadValueFromMysql(USER_INFO &user_info);//从数据库读取人员所有信息
    int GetCountFromMysql();//获取人员总数
    QString GetPasswordFromMysql(QString name);//根据用户名从数据库获取密码
    QString GetuserFromMysql(QString name); //根据电话或emil获取用户名
//笔记本
    void AddNotesToMysql(NOTES note, QString tables);//向数据库加入笔记(笔记标题和内容)
    void AddBooksFromMysql(QString book, QString user);//根据用户名建立表并添加笔记本名
    void GetAllBook(QString user);//用户获取已经建立的笔记本
    QString Getbooklist(int i);
    int GetNumOfBook(QString user);//获取某个用户创建的笔记本个数
    void ReadNotesFromMysql( QString user);//获取笔记标题以及内容
    NOTES GetNotes(int i);
    int GetNumOfNote(QString book);
    QString GetQueryInfo(QString note, QString user);//根据搜索框内容查询笔记
    QString GetEmil(QString tel);
    void ModifyPassword(QString tel, QString password);
    ~Storage();
private:
    Storage();
    MYSQL_INFO mysql_info;
    USER_INFO user_info;
    QString booklist[NUM];
    NOTES note[NUM];
};

#endif // STORAGE_H
