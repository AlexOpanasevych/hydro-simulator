#include "devlogger.h"

#include <QGuiApplication>
#include <QTimer>

DevLogger::DevLogger(QObject *parent) : QObject(parent)
{
    connect(this, &DevLogger::addLogSignal, this, [this](QString category, QString functionName, QString msg){
        while(msg.contains("\033[")){
            int idx = msg.indexOf("\033[");
            msg.remove(idx, 5);
        }
        while (m_logText.count("<br>") > 50){
            int idx = m_logText.indexOf("<br>");
            m_logText.remove(0, idx + 4);
        }
        setLogText(m_logText + "<font color=\"#e94040\">" + category + "</font><font color=\"#8ECECB\">[" + functionName + "]</font>: </font><font color=\"#1f2936\">" + msg + "</font><br>");
    });

    connect(this, &DevLogger::addFancyLogSignal, this, [this](QString category, QString functionName, QString msg){
        Q_UNUSED(category)
        Q_UNUSED(functionName)
//        while(msg.contains("\033[")){
//            int idx = msg.indexOf("\033[");
//            msg.remove(idx, 5);
//        }
//        while (m_fancyLogText.count("<br>") > 50){
//            int idx = m_fancyLogText.indexOf("<br>");
//            m_fancyLogText.remove(0, idx + 4);
//        }
        setFancyLogText(/*m_fancyLogText + *//*"<font color=\"#e94040\">" + category + "</font><font color=\"#8ECECB\">[" + functionName + "]</font>: </font><font color=\"#1f2936\">" + */msg/* + "</font><br>"*/);
    });
}

void DevLogger::addLog(QString category, QString functionName, QString msg)
{
    if (available())
        emit addLogSignal(category, functionName, msg);
}

void DevLogger::addFancyLog(QString category, QString functionName, QString msg)
{
     if (available())
         emit addFancyLogSignal(category, functionName, msg);
}
