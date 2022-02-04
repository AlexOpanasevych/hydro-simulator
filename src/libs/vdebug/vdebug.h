#ifndef VDEBUG_H
#define VDEBUG_H

#include "VDebug_global.h"
#include <QString>
#include <QByteArray>
#include <QDebug>

class VDEBUG_EXPORT VDebug
{
public:
    void static Error(const QString msg);
    void static Warning(const QString msg);
    void static Info(const QString msg);
    void static Success(const QString msg);

    void static Hex(const QByteArray arr);
    void static Hex(const QString desc, const QByteArray arr);
};

#endif // VDEBUG_H
