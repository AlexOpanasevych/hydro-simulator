#ifndef NOV_PACKET_PARSER
#define NOV_PACKET_PARSER

#include <QByteArray>


class NovPacketParser
{
public:
    // get -----------------------
    static quint8 getBid();
    static quint8 getRamCmd();

    static QByteArray nvo_par_send(quint8 tokenId, quint32 generationTimeMs, quint32 latencyTimeMs, bool IsSerialPattern, quint8 pattern, quint8 filtWindow);
    static QByteArray debugGenerationSend(quint8 tokenId, bool enable);
    static QByteArray startOnceMeasure(quint8 tokenId);
    static QByteArray streamEnable(quint8 tokenId, bool enable);
    static QByteArray setFirstOrderFilter(quint8 tokenId, quint16 value);
    static QByteArray setSeconOrderFilter(quint8 tokenId, quint16 value);

    static QByteArray setDebugEnable(quint8 tokenId, bool enable, bool debugMode);
    static QByteArray BOOOOOOOOOOOOOOOOM(quint8 tokenId, bool boooomEnable);

    static QByteArray startSelfWriter(quint8 tokenId, int selfMode);
};

#endif // NOV_PACKET_PARSER
