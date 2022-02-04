#include "serversideparser.h"


ServerSideParser::ServerSideParser()
{
}

int ServerSideParser::parse(QDataStream *s) {

    return 1;
}

QVariantList ServerSideParser::captured()
{
    return {};
}
