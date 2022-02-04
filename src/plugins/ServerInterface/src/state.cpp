#include "state.h"
#include "station.h"
#include "serverservice.h"
#include "MessageType.h"
#include <cdebug.h>

void Resource::setStn(Station *value)
{
    stn = value;
}

Resource::Resource(QObject * parent) : QObject(parent)
{

}

void Resource::onPropertyChanged(QString propName, QVariant prop)
{
    QVariantMap result;
    if(m_value.canConvert<QVariantMap>()) {
        result = m_value.toMap();
    }
    result[propName] = prop;
    setValue(result);
    QByteArray arr;
    QDataStream ss(&arr, QIODevice::WriteOnly);
    QVariantMap m;
    m[propName] = prop;
    qDebug(s) << "property changed, sending value" << m << m_client;
    qDebug(s) << "client" << stn->currentClient();
    ss << STATE_CHANGED << m_stateId << m_client << QVariant(m);
    stn->getService()->worker.sendDataByID(stn->currentClient(), arr);
}
