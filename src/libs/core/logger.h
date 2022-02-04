#ifndef LOGGER_H
#define LOGGER_H
#include <QtConcurrent>
#include <QtSql/QSqlDatabase>
#include "core_global.h"


class SampleDataContainer;
class CORE_LIBRARY_EXPORT Logger
{
public:
    Logger(SampleDataContainer* new_container);
    ~Logger();
    void startProceed(QString processDescription);
    void requestStop(std::function<void(void)> callback);
protected:
    bool is_fully_logged();
    void proceed();
private:
    void initLogger(QString processDescription);
    void finishLogger();
    SampleDataContainer* container;
    QSqlDatabase db;
    QString start_time , finish_time;
    int processId;
    bool is_work = false;
    bool m_stopRequest = false;
    std::function<void(void)> m_stopCallback = nullptr;
    QFuture<void> future;
    struct FileStreamData
    {
        QFile *file;
        QDataStream *stream;
    };
    std::map<QString, FileStreamData> m_streams;
};


#endif // LOGGER_H
