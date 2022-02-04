#include "vdebug.h"

void VDebug::Error(const QString msg)
{
    qDebug().noquote() << "\033[31m[ ERROR ] " << msg << "\033[0m";
}

void VDebug::Warning(const QString msg) {
    qDebug().noquote() << "\u001b[33m[ WARNING ] " << msg << "\033[0m";
}

void VDebug::Info(const QString msg) {
    qDebug().noquote() << "\u001b[34m[ INFO ] " << msg << "\033[0m";
}

void VDebug::Success(const QString msg) {
    qDebug().noquote() << "\u001b[32m[ OK ] " << msg << "\033[0m";
}

void VDebug::Hex(const QByteArray arr)
{
    qDebug().noquote() << arr.toHex(' ');
}

void VDebug::Hex(const QString desc, const QByteArray arr)
{
    qDebug().noquote() << desc << arr.toHex(' ');
}
