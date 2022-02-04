#ifndef CLIENTPARSER_H
#define CLIENTPARSER_H

#include "ClientParser_global.h"

#include <QDataStream>
#include <QVariantList>
/*
    so this is interface for intercommunication data parsers between main server and clients qml
*/

class CLIENTPARSER_EXPORT IDataParser
{
public:
    IDataParser();
    virtual int parse(QDataStream *s) = 0;
    virtual QVariantList captured() = 0;
};

#endif // CLIENTPARSER_H
