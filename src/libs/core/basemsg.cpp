#include "basemsg.h"

#include <QDataStream>
#include <QDebug>

BaseMsg::BaseMsg()
{
    instanceCount++;
    //qDebug() << "count of msg instances" << instanceCount;
}

BaseMsg::BaseMsg(const BaseMsg & msg)
{
    Q_UNUSED(msg)
    this->instanceCount++;
}

BaseMsg::~BaseMsg(){
    instanceCount--;
    //qDebug() << "count of msg instances" << instanceCount;
}


QDataStream &operator<<(QDataStream &out, const Header& obj) {
   out << obj.isAnswer << obj.type << obj.uid << obj.itemId;
   return out;
}

QDataStream &operator>>(QDataStream &in, Header &obj) {
   in >> obj.isAnswer >> obj.type >> obj.uid >> obj.itemId;
   return in;
}
