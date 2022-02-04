/*!
    \brief Tool for simple read stream data
    \authors oleksii, Shpegun60
*/

#ifndef CONVERT_H
#define CONVERT_H

#include <QDebug>
#include <QByteArray>
#include <KBus_global.h>

class KBUS_EXPORT Convert
{
public:
    /*!
        \brief Most significant bit (MSB)
    */
    struct KBUS_EXPORT FB {
        static quint8 readU8(const QByteArray&, int pos);
        static void writeU8(QByteArray&, int pos, quint8 val);

        static quint16 readU16(const QByteArray&, int pos);
        static void writeU16(QByteArray&, int pos, quint16 val);

        static quint32 readU24(const QByteArray&, int pos);
        static void writeU24(QByteArray&, int pos, quint32 val);
        static qint32 readI24(QByteArray &a, int pos);
        static qint32 readI32(const QByteArray&, int pos);
        static void writeI32(QByteArray&, int pos, qint32 val);

        static quint32 readU32(const QByteArray &a, int pos);
    };

    /*!
        \brief Least significant bit (LSB)
    */
    struct KBUS_EXPORT FL {
        static quint8 readU8(const QByteArray&, int &pos);
        static void writeU8(QByteArray&, int pos, quint8 val);

        static quint16 readU16(const QByteArray&, int &pos);
        static void writeU16(QByteArray&, int pos, quint16 val);

        static quint32 readU24(const QByteArray&, int &pos);
        static void writeU24(QByteArray&, int pos, quint32 val);

        static qint32 readI32(const QByteArray&, int &pos);
        static void writeI32(QByteArray&, int pos, qint32 val);
    };


    static void setBit(quint8 * byte, int pos, bool value);
};

#endif // CONVERT_H
