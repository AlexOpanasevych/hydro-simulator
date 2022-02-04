#include "logger.h"
#include "sampledatacontainer.h"
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlField>
Logger::Logger(SampleDataContainer *new_container)
{
    container = new_container;
    db = QSqlDatabase::addDatabase("QSQLITE",  "eeeee");
}

Logger::~Logger()
{

}
bool Logger::is_fully_logged()
{
    for (int piDescriptorIndex=0;piDescriptorIndex<int(container->set_of_piDescriptors.size());piDescriptorIndex++)
    {
        int piDescriptor = container->set_of_piDescriptors[piDescriptorIndex];
        for(int index=0;index<int(container->data[piDescriptor].size());index++)
        {
            if (!container->data[piDescriptor][index].buffer->isFPosEqualToWritePos())
            {
                return false;
            }
        }
    }
    return true;
}
void Logger::proceed()
{
    for (int indexPiDescriptor=0;indexPiDescriptor<int(container->set_of_piDescriptors.size());indexPiDescriptor++)
    {
        int piDescriptor = container->set_of_piDescriptors[indexPiDescriptor];
        int numOfRecords = container->data[piDescriptor].size();
        if (numOfRecords == 0) continue;
        for (int RecordPosition = 0; RecordPosition<numOfRecords ;RecordPosition++)
        {
            QString s = QString::fromStdString("data/")+QString::fromStdString(std::to_string(processId))+"/"+QString::fromStdString(std::to_string(piDescriptor))+QString::fromStdString("_")+ container->data[piDescriptor][RecordPosition].SDType +QString::fromStdString("_")+QString::fromStdString(std::to_string(RecordPosition)) +QString::fromStdString(".data");
            auto search = m_streams.find(s);
            FileStreamData sd;
            if (search == m_streams.end())
            {
                sd.file = new QFile(s);
                sd.file->open(QFile::Append);
                sd.stream = new QDataStream(sd.file);
                m_streams.insert({s, sd});
                QString a = QString::fromStdString(std::to_string(piDescriptor)) , b = container->data[piDescriptor][RecordPosition].SDType , c = QString::fromStdString(std::to_string(processId)) , d = QString::fromStdString(std::to_string(RecordPosition));
                QSqlQuery q(db);
                s = ("insert into PhysicalItem (piDescriptor ,RecordPosition ,type , processid) VALUES (" + a +","+d+","+b+","+c+");");
                if(!q.exec(s))
                {
                    qDebug() << "Error: " <<  q.lastError();
                }
                int idd = 0;
                QString ss = "SELECT id FROM PhysicalItem WHERE id = (SELECT MAX(id)  FROM PhysicalItem);";
                if(!q.exec(ss))
                {
                    qDebug() << "Error" << q.lastError();
                }
                q.next();
                idd = q.record().value(0).toInt();
                q.prepare("insert into DataFiles (Pid ,RecordPosition ,  Filename) VALUES ("+QString::fromStdString(std::to_string(idd))+","+QString::fromStdString(std::to_string(RecordPosition))+",:name);");
                QString nn =QString("data/")+QString::fromStdString(std::to_string(processId))+ "/" + QString::fromStdString(std::to_string(piDescriptor))+QString("_")+container->data[piDescriptor][RecordPosition].SDType+QString("_")+QString::fromStdString(std::to_string(RecordPosition))+QString(".data");
                q.bindValue(":name" , nn);
                if(!q.exec())
                {
                    qDebug() << "Error" << q.lastError();
                }
            }
            else
            {
                sd = search->second;
            }
            std::vector<SampleData*> logPiece = container->data[piDescriptor][RecordPosition].buffer->moveToWritePos();
            for (auto k=0; k<int(logPiece.size()); k++) {
                logPiece[k]->serialize(sd.stream);
            }
            sd.file->flush();
        }
    }
}
void Logger::initLogger(QString processDescription)
{
    //create data dir
    QDir dir;
    if (!dir.exists("data"))
    {
        dir.mkdir("data");
    }
    start_time = QDateTime::currentDateTime().toString();
    if (db.isValid()) {
        db.close();
    }
    // open db and create required tables
    QString path = QDir::currentPath()+"/data/";
    db.setDatabaseName(path + "DataBase.db");
    db.open();
    QSqlQuery q(db);
    if(!q.exec("CREATE TABLE IF NOT EXISTS Process (`id`  INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,`name`  TEXT,`start`  TEXT,`finish`  TEXT);"))
    {
        qDebug() << "Error1 " << q.lastError();
    }
    if(!q.exec("CREATE TABLE IF NOT EXISTS DataFiles (`id`  INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,`PiD`  INTEGER ,`RecordPosition` INTEGER,`Filename` TEXT);"))
    {
        qDebug() << "Error2 " << q.lastError();
    }
    if(!q.exec("CREATE TABLE IF NOT EXISTS PhysicalItem (`id`  INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,`piDescriptor`  INTEGER,`RecordPosition` INTEGER,`type`  INTEGER,`processid`  INTEGER);"))
    {
        qDebug() << "Error3 " << q.lastError();
    }
    //add record to process table
    finish_time = "-1";
    q.prepare("insert into Process (name , start , finish) VALUES (:ban ,:start , :finish);");
    q.bindValue(":start" , start_time);
    q.bindValue(":finish" , finish_time);
    q.bindValue(":ban" , processDescription);
    q.exec();
    if(!q.exec("SELECT id FROM Process WHERE id = (SELECT MAX(id)  FROM Process);"))
    {
        qDebug() << "Error4" << q.lastError();
    }
    q.next();
    processId = q.record().value(0).toInt();
    //make dir for data files
    dir.mkdir( path + QString::number(processId));
}

void Logger::finishLogger()
{
    finish_time = QDateTime::currentDateTime().toString();
    db.open();
    QSqlQuery q(db);
    q.prepare("UPDATE Process SET finish = :fin WHERE id = :id;");
    q.bindValue(":fin" , finish_time);
    q.bindValue(":id" , processId);
    if (!q.exec()) {
        qDebug() << "Error6" << q.lastError();
    }
}

void Logger::startProceed(QString processDescription)
{
    if (is_work) {
        qDebug() << "CAN'T START LOGGER UNTIL PREVIOUS WORKING";
        return;
    }
    is_work = true;
    future = QtConcurrent::run([this, processDescription]() {
        initLogger(processDescription);
        while (is_work) {
            proceed();
            if (m_stopRequest) {
                if (is_fully_logged()) {
                    break;
                }
            }
            QThread::msleep(1);
        }
        finishLogger();
        if(m_stopCallback != nullptr) m_stopCallback();
        m_stopCallback = nullptr;
        is_work = false;
        m_stopRequest = false;
    });
}

void Logger::requestStop(std::function<void ()> callback)
{
    if (m_stopRequest) return;
    m_stopCallback = callback;
    m_stopRequest = true;
    future.waitForFinished();
}

