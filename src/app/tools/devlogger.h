#ifndef DEVLOGGER_H
#define DEVLOGGER_H

#include <QObject>
#include <kmacro.h>
#include <utility/kobservablelist.h>
#include <QElapsedTimer>
#include <models/kflexiblemodel.h>
#include <cdebug.h>

class QTimer;
class DevLogger : public QObject
{
    Q_OBJECT
    K_SINGLETON(DevLogger)
    K_QML_SINGLETON(DevLogger)
    K_READONLY_PROPERTY(QString, logText, logText, setLogText, logTextChanged, "")
    K_READONLY_PROPERTY(QString, fancyLogText, fancyLogText, setFancyLogText, fancyLogTextChanged, "")
    K_AUTO_PROPERTY(bool, available, available, setAvailable, availableChanged, true)
    public:
        explicit DevLogger(QObject *parent = nullptr);
    ~DevLogger() {qDebug(s) << "devlogger destructor";}
    void addLog(QString category, QString functionName, QString msg);
    void addFancyLog(QString category, QString functionName, QString msg);
signals:
    void addLogSignal(QString category, QString functioName, QString msg);
    void addFancyLogSignal(QString category, QString functioName, QString msg);
private:
    void proceed();
};


#endif // DEVLOGGER_H
