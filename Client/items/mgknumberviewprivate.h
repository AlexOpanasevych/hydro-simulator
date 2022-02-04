#ifndef MGKNUMBERVIEWPRIVATE_H
#define MGKNUMBERVIEWPRIVATE_H

//#include <QQuickItem>
#include <kmacro.h>
#include <QVariant>
#include <QMutex>
#include <streamconnector.h>

class MGKNumberViewPrivate : public QObject
{
    Q_OBJECT
    K_QML_TYPE(MGKNumberViewPrivate)
    K_AUTO_PROPERTY(QVariant, value, value, setValue, valueChanged, QVariant())
public:
    MGKNumberViewPrivate();
    ~MGKNumberViewPrivate();
    listener m_handler;
signals:

};

#endif // MGKNUMBERVIEWPRIVATE_H
