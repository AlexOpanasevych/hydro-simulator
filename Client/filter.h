#ifndef FILTER_H
#define FILTER_H

#include <QObject>
#include <kmacro.h>

class Filter : public QObject
{
    Q_OBJECT
    K_QML_TYPE(Filter)
    K_AUTO_PROPERTY(QString, propertyName, propertyName, setPropertyName, propertyNameChanged, "")
    K_AUTO_PROPERTY(QVariant, value, value, setValue, valueChanged, QVariant())

public:
    explicit Filter(QObject *parent = nullptr);
    virtual bool filter(QObject * obj);

signals:

};

#endif // FILTER_H
