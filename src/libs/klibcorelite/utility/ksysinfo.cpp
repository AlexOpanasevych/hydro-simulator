#include "ksysinfo.h"

#include <QSysInfo>

KSysInfo::KSysInfo(QObject *parent) : QObject(parent) {
    setMachineUniqueId(QSysInfo::machineUniqueId());
    setMachineHostName(QSysInfo::machineHostName());
}
