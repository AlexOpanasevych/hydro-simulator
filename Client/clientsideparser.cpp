#include "clientsideparser.h"

#include <QDataStream>

ClientSideParser::ClientSideParser(uint8_t typeOfData)
{
    this->typeOfData = typeOfData;
}

int ClientSideParser::parse(QDataStream *s)
{
//    uint8_t typeOfData;
//    *s >> typeOfData;
    miniParsers[typeOfData](s, &result);
    return typeOfData;
}

QVariantList ClientSideParser::captured()
{

    return result;
}
