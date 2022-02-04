#include "mgkitembase.h"
#include "statecommunicator.h"

MGKItem::MGKItem(QObject *parent) : QObject(parent)
{
}

MGKItem::~MGKItem()
{
}



void deserialize(QDataStream *)
{

}

void serialize(QDataStream *)
{

}

void MGKItem::init(QVariantMap prop) {
    setX(prop["x"].toInt());
    setY(prop["y"].toInt());
    setHeight(prop["height"].toInt());
    setWidth(prop["width"].toInt());
    setCaption(prop["caption"].toString().replace("\\n", "<br>").replace("\n", "<br>"));
    setGlobalIndex(prop["id"].toInt());
    setAdditionalQml(prop["additionalQml"].toMap());
    setControlType(prop["controlType"].toString());
    auto color = prop["color"].toString();
    if(!color.isEmpty())
        setColor(QColor(color));
    setClientName(prop["name"].toString());
//    QList<State*> states;
//    QVariantList stateList = prop["action"].toList();
    Resource * state = new Resource(this);
    state->setStateId(globalIndex());
    state->setStation(clientName().chopped(1));
    state->setName(prop["controlName"].toString());
    setResource(state);
}

Resource::Resource(QObject *parent) : QObject(parent)
{

}

void MGKItem::setValue(QVariant value)
{
    this->m_value = value;
    emit valueChanged(m_value);
}

QVariant MGKItem::value()
{
    return m_value;
}
