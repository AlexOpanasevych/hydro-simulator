#ifndef CUSOMMONITOR_H
#define CUSOMMONITOR_H

#include <QObject>
#include <QTimer>

#include <kmacro.h>

class CustomMonitor : public QObject
{
    Q_OBJECT
    K_READONLY_PROPERTY(QString, text, text, setText, textChanged, "")
public:
    explicit CustomMonitor(QObject *parent = nullptr);

signals:
private:
    QTimer timer;
};

#endif // CUSOMMONITOR_H
