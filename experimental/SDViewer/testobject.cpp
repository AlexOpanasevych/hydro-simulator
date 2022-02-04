#include "testobject.h"
#include <QUrl>
#include <QSqlTableModel>
#include <QSqlError>
TestObject::TestObject(QObject *parent) : QObject(parent)
{

}
void TestObject::load()
{
    bool turn = false;
    db =  QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("data/DataBase.db");
    if(!db.open())
    {
        qDebug() << "not open";
    }
    setProcesses(processesLst.model());
    processesLst.clear();
    QSqlQuery q(db);
    if(!q.exec("SELECT * from Process;"))
    {
        qDebug() << "ERROORR " << q.lastError();
    }
    int id;
    QString name , start , finish;
    QSqlRecord rec = q.record();
    Process **p = new Process* [100];
    int i=0;
    while(q.next())
    {
        turn^=1;
        p[i] = new Process;
        id = q.value(rec.indexOf("id")).toInt();
//        qDebug() << id;
        name = q.value(rec.indexOf("name")).toString();
        start = q.value(rec.indexOf("start")).toString();
        finish = q.value(rec.indexOf("finish")).toString();
        p[i]->setName(name);
        p[i]->setProcid(id);
        p[i]->setStart(start);
        p[i]->setFinish(finish);
        p[i]->setWhichTurn(turn);
        processesLst.push_back(p[i]);
        ++i;
    }
    db.close();
}
void TestObject::push(QString p)
{
    auto p3 = new Process;
    p3->setName(p);
    processesLst.push_back(p3);

}
Process::Process(QObject *parent): QObject(parent)
{
    setPi(pilst.model());
}
void Process::load()
{
    bool turn = false;
    db =  QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("data/DataBase.db");
    if(!db.open())
    {
        qDebug() << "not open";
    }
    pilst.clear();
    setProcid(procid());
    QSqlQuery q(db);
    std::set<int> set_of_pid;
    std::vector<int> vec_of_pid;
    if(!q.exec("SELECT * FROM PhysicalItem WHERE processid = "+QString::fromStdString(std::to_string(procid())) + ";"))
    {
        qDebug() << "ERRORR" << q.lastError();
    }
    QSqlRecord rec = q.record();
    PI **p = new PI* [10];
    int id ,  pid;
    int i=0;
    while(q.next())
    {
        turn^=1;
        p[i] = new PI;
        id = q.value(rec.indexOf("id")).toInt();
        pid = q.value(rec.indexOf("piDescriptor")).toInt();
        int size_before = set_of_pid.size();
        set_of_pid.insert(pid);
        int size_after = set_of_pid.size();
        if(size_after!=size_before){
            p[i]->setPid(pid);
            p[i]->setId(id);
            p[i]->setProcid(this->procid());
            p[i]->setName(this->name());
            p[i]->setStart(this->start());
            p[i]->setFinish(this->finish());
            p[i]->setWhichTurn(turn);
            pilst.push_back(p[i]);
        }
        ++i;
    }
    db.close();
}

PI::PI(QObject *parent) : QObject(parent)
{
    setSd(sdlist.model());
}

void PI::load()
{
    bool turn = false;
    db =  QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("data/DataBase.db");
    if(!db.open())
    {
        qDebug() << "not open";
    }
    sdlist.clear();
    setSd(sdlist.model());
    QSqlQuery q(db);
    if(!q.exec("SELECT * FROM PhysicalItem  WHERE processid = "+QString::fromStdString(std::to_string(procid()))+ " and piDescriptor = " +QString::fromStdString(std::to_string(pid())) +";"))
    {
        qDebug() << "ERROR" << q.lastError();
    }
    QSqlRecord rec = q.record();
    SD ** p =new  SD* [100];
    int i = 0;
    int type;
    int recordposition;
    while(q.next())
    {
        turn^=1;
        p[i]=new SD;
        type = q.value(rec.indexOf("type")).toInt();
        recordposition = q.value(rec.indexOf("RecordPosition")).toInt();
        p[i]->setType(type);
        p[i]->setRecordposition(recordposition);
        p[i]->setPid(this->pid());
        p[i]->setId(this->id());
        p[i]->setProcid(procid());
        p[i]->setName(this->name());
        p[i]->setStart(this->start());
        p[i]->setFinish(this->finish());
        p[i]->setWhichTurn(turn);
        sdlist.push_back(p[i]);
        i++;
    }
    db.close();
}
