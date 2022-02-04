#ifndef SERVERSIDEPARSER_H
#define SERVERSIDEPARSER_H

#include <idataparser.h>
#include "../serverinterface_global.h"

class SERVERINTERFACE_LIBRARY_EXPORT ServerSideParser : public IDataParser
{
    QByteArray data;
public:
    ServerSideParser();

    //!
    //! \brief Parses data from client
    //! \param s
    //! \return code of error, 0 if succeed
    //!
    int parse(QDataStream *s) override;

    //!
    //! \brief captured
    //! \return captured data list
    //!
    QVariantList captured() override;
};

#endif // CLIENTSIDEPARSER_H
