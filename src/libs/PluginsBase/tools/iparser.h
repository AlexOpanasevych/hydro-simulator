#ifndef IPARSER_H
#define IPARSER_H

#include "cmd.h"

class PLUGINSBASE_EXPORT IParser
{
public:
    virtual QList<Cmd*> *parse(QString text, bool *isError = nullptr, QString *errorString = nullptr) = 0;
};

#endif // IPARSER_H
