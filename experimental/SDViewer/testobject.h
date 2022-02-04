#ifndef TESTOBJECT_H
#define TESTOBJECT_H
#include <QDebug>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlField>
#include <QObject>
#include <kmacro.h>
#include <utility/kobservablelist.h>
#include <QDir>
#include <set>

class SD : public QObject
{
    Q_OBJECT
    K_AUTO_PROPERTY(int , type , type , setType , typeChanged , -1)
    K_AUTO_PROPERTY(int , recordposition , recordposition , setRecordposition , recordpositionChanged , -1)
    K_AUTO_PROPERTY(int, pid, pid, setPid, pidChanged,-1)
    K_AUTO_PROPERTY(QString,name , name , setName ,nameChanged , "")
    K_AUTO_PROPERTY(QString,start , start , setStart ,startChanged , "")
    K_AUTO_PROPERTY(QString,finish , finish , setFinish ,finishChanged , "")
    K_AUTO_PROPERTY(int,id , id , setId ,idChanged , -1)
    K_AUTO_PROPERTY(int,procid , procid , setProcid ,procidChanged , -1)
    K_AUTO_PROPERTY(bool , whichTurn , whichTurn , setWhichTurn , whichTurnChanged , false);
};

class PI : public QObject
{
    Q_OBJECT
    K_AUTO_PROPERTY(int, pid, pid, setPid, pidChanged,-1)
    K_AUTO_PROPERTY(QString,name , name , setName ,nameChanged , "")
    K_AUTO_PROPERTY(QString,start , start , setStart ,startChanged , "")
    K_AUTO_PROPERTY(QString,finish , finish , setFinish ,finishChanged , "")
    K_AUTO_PROPERTY(int,id , id , setId ,idChanged , -1)
    K_AUTO_PROPERTY(KObservableModel*, sd, sd, setSd, sdChanged, nullptr)
    K_AUTO_PROPERTY(int,procid , procid , setProcid ,procidChanged , -1)
    K_AUTO_PROPERTY(bool , whichTurn , whichTurn , setWhichTurn , whichTurnChanged , false);
    public:
        explicit PI(QObject *parent = nullptr);
public slots:
    void load();
private:
    KObservableList<SD> sdlist;
    QSqlDatabase db;
};

class Process : public QObject
{
    Q_OBJECT
    K_AUTO_PROPERTY(KObservableModel*, pi, pi, setPi, piChanged, nullptr)
    K_AUTO_PROPERTY(QString,name , name , setName ,nameChanged , "")
    K_AUTO_PROPERTY(QString,start , start , setStart ,startChanged , "")
    K_AUTO_PROPERTY(QString,finish , finish , setFinish ,finishChanged , "")
    K_AUTO_PROPERTY(int,procid , procid , setProcid ,procidChanged , -1)
    K_AUTO_PROPERTY(bool , whichTurn , whichTurn , setWhichTurn , whichTurnChanged , false);
    public:
        explicit Process(QObject *parent = nullptr);

public slots:
    void load();
private:
    KObservableList<PI> pilst;
    QSqlDatabase db;
};

class TestObject : public QObject
{
    Q_OBJECT
    K_SINGLETON(TestObject)
    K_QML_SINGLETON(TestObject)
    K_AUTO_PROPERTY(KObservableModel*, processes, processes, setProcesses, processesChanged, nullptr)
    K_AUTO_PROPERTY(QAbstractTableModel*, table,table,setTable,onTableChanged, nullptr)
    explicit TestObject(QObject *parent = nullptr);
public:
signals:
public slots:
    void load();
    void push(QString);
private:
    KObservableList<Process> processesLst;
    QSqlDatabase db;
};
#endif // TESTOBJECT_H
