#ifndef CDEBUG_H
#define CDEBUG_H

#include <QLoggingCategory>
#include <QDebug>

#if defined(QT_NO_DEBUG_OUTPUT)
#  undef qDebug
#  define qDebug QMessageLogger(QT_MESSAGELOG_FILE, QT_MESSAGELOG_LINE, QT_MESSAGELOG_FUNC).debug
#endif

static const QLoggingCategory &v()
{
    static const QLoggingCategory category("vitaliy");
    return category;
}

static const QLoggingCategory &s()
{
    static const QLoggingCategory category("sasha");
    return category;
}

static const QLoggingCategory &k()
{
    static const QLoggingCategory category("kostia");
    return category;
}

static const QLoggingCategory &g()
{
    static const QLoggingCategory category("global");
    return category;
}

static const QLoggingCategory &l()
{
    static const QLoggingCategory category("log");
    return category;
}

#endif // CDEBUG_H
