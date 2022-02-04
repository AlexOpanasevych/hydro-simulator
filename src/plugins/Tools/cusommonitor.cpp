#include "cusommonitor.h"

#include <QGuiApplication>
#include <QProcess>

CustomMonitor::CustomMonitor(QObject *parent) : QObject(parent)
{
    connect(&timer, &QTimer::timeout, this, [this](){
        QProcess p;
        p.start("ps", QStringList() << "-p" << QString::number(QGuiApplication::applicationPid()) << "-o" << "%cpu,%mem");
        p.waitForFinished();
        QString memory = p.readAllStandardOutput();
        qDebug() << memory << QString::number(QGuiApplication::applicationPid());
        //setText(QString("; RAM: %1 MB").arg(memory.toLong() / 1024));
        p.close();
    });
    //timer.start(100);

    //SYSTEM_INFO systemInfo;
    //GetSystemInfo(&systemInfo);
    // qDebug () << "Processor Mask" << systemInfo.dwActiveProcessorMask;
    // qDebug () << "Number of Processors" << systemInfo.dwNumberOfProcessors;
    // qDebug () << "Processor Page Size" << systemInfo.dwPageSize;
    // qDebug () << "Processor Type" << systemInfo.dwProcessorType;
    // qDebug () << "Maximum Addressable Unit" << systemInfo.lpMaximumApplicationAddress;
    // qDebug () << "Minimum Addressing Unit" << systemInfo.lpMinimumApplicationAddress;
    // qDebug () << "Processor Level" << systemInfo.wProcessorLevel;
    // qDebug () << "Processor Version" << systemInfo.wProcessorRevision;
    //
    // MEMORYSTATUSEX statusex;
    // statusex.dwLength=sizeof(statusex);
    // GlobalMemoryStatusEx(&statusex);
    //  qDebug () << "Physical Memory Usage" << statusex.dwMemoryLoad;
    //  qDebug () << "Total Physical Memory" << statusex.ullTotalPhys / 1024/1024;
    //  qDebug () << "Available Physical Memory" << statusex.ullAvailPhys / 1024/1024;
    //  qDebug () << "System Page File Size" << statusex.ullTotalPageFile / 1024/1024;
    //  qDebug () << "System Available Page File Size" << statusex.ullAvailPageFile / 1024/1024;
    //  qDebug () << "Total Virtual Memory" << statusex.ullTotalVirtual / 1024/1024;
    //  qDebug () << "Available Virtual Memory" << statusex.ullAvailVirtual / 1024/1024;
    //  qDebug () << "Reserved (value is 0)" << statusex.ullAvailExtendedVirtual / 1024/1024;
}
