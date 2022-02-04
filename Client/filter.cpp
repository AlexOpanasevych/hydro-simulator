#include "filter.h"

Filter::Filter(QObject *parent) : QObject(parent)
{

}

bool Filter::filter(QObject *obj)
{
    return obj->property(m_propertyName.toUtf8().constData()) == m_value;
}
