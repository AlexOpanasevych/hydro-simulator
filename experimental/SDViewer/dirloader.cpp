#include "dirloader.h"
#include <QUrl>
#include <QSqlTableModel>
#include <QSqlError>
DirLoader::DirLoader(QObject *parent) : QObject(parent)
{
    db =  QSqlDatabase::addDatabase("QSQLITE");
    setProcesses(processesLst.model());
}
void DirLoader::load(QString path)
{
    if (path.startsWith("file:///")) path.remove("file:///");
    setPath(path);
    if (db.open()) db.close();
    db.setDatabaseName(path+"/DataBase.db");

    if(!db.open())
    {
        qDebug() << "not open";
    }

    processesLst.clear();
    QSqlQuery q(db);
    if(!q.exec("SELECT * from Process;"))
    {
        qDebug() << "ERROORR " << q.lastError();
    }
    int id;
    QString name , start , finish;
    QSqlRecord rec = q.record();
    while(q.next())
    {
        auto p = new Process;
        id = q.value(rec.indexOf("id")).toInt();
        name = q.value(rec.indexOf("name")).toString();
        start = q.value(rec.indexOf("start")).toString();
        finish = q.value(rec.indexOf("finish")).toString();
        p->setName(name);
        p->setProcid(id);
        p->setStart(start);
        p->setFinish(finish);
        p->dirPath = path;
        processesLst.push_back(p);
    }
}
void DirLoader::push(QString p)
{
    auto p3 = new Process;
    p3->setName(p);
    processesLst.push_back(p3);

}

void DirLoader::clear()
{
    processesLst.clear();
}
Process::Process(QObject *parent): QObject(parent)
{
    setPi(pilst.model());
}
void Process::load()
{
    pilst.clear();
    setProcid(procid());
    QSqlQuery q;
    if(!q.exec(QString("SELECT * FROM PhysicalItem WHERE processid = %1;").arg(procid())))
    {
        qDebug() << "ERRORR" << q.lastError();
    }
    QSqlRecord rec = q.record();
    int id ,  pid;
    QSet<int> loadedPis;
    while(q.next())
    {
        id = q.value(rec.indexOf("id")).toInt();
        pid = q.value(rec.indexOf("piDescriptor")).toInt();
        if (loadedPis.contains(pid))
            continue;
        loadedPis.insert(pid);
        auto p = new PI(this);
        p->setPid(pid);
        p->setId(id);
        p->setProcid(this->procid());
        p->setName(this->name());
        p->setStart(this->start());
        p->setFinish(this->finish());
        p->dirPath = dirPath;
        pilst.push_back(p);
    }
}

PI::PI(QObject *parent) : QObject(parent)
{
    setSd(sdlist.model());
}

void PI::load()
{
    sdlist.clear();
    setSd(sdlist.model());
    QSqlQuery q;
    if(!q.exec("SELECT * FROM PhysicalItem  WHERE processid = "+QString::fromStdString(std::to_string(procid()))+ " and piDescriptor = " +QString::fromStdString(std::to_string(pid())) +";"))
    {
        qDebug() << "ERROR" << q.lastError();
    }
    QSqlRecord rec = q.record();
    int type;
    int recordposition;
    while(q.next())
    {
        auto p = new SD(this);
        type = q.value(rec.indexOf("type")).toInt();
        recordposition = q.value(rec.indexOf("RecordPosition")).toInt();
        p->setType(type);
        p->setRecordposition(recordposition);
        p->setPid(this->pid());
        p->setId(this->id());
        p->setProcid(procid());
        p->setName(this->name());
        p->setStart(this->start());
        p->setFinish(this->finish());
        p->filePath = dirPath + QString("/%1/%2_%3_%4.data").arg(procid()).arg(pid()).arg(type).arg(recordposition);
        sdlist.push_back(p);
    }
}
