#include "client.h"

//QString Client::toString() {
//    switch (m_type) {
//    case 0:
//        return "Platina" + QString::number(m_relativeId);
//    case 1:
//        return "Bronze" + QString::number(m_relativeId);
//    default: {
//        qDebug () << "unknown type : check your code";
//        return "";
//    }
//    }
//}

Client::Client(QObject *parent) : QObject(parent)
{
}

Client::~Client()
{

}
