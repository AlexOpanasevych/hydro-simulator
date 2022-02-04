#include "convert.h"
#include "cdebug.h"

quint8 Convert::FB::readU8(const QByteArray &a, int pos)
{
    if(pos>=a.size()){
        qDebug()<<"CONVERT: Out of range";
        return 0x00;
    }
    return a[pos];
}

void Convert::FB::writeU8(QByteArray &a, int pos, quint8 val)
{
    if(pos>=a.size()){
        qDebug()<<"CONVERT: Out of range";
        return;
    }
    a[pos] = val;
}

quint16 Convert::FB::readU16(const QByteArray &a, int pos)
{
    quint32 res = 0x00;
    if(pos+1>=a.size()){
        qDebug()<<"CONVERT: Out of range";
        return 0x00;
    }


    res |= (a[pos] << 8 ) & 0xFF00;
    res |=  a[pos+1]        & 0x00FF;


    return res;
}

void Convert::FB::writeU16(QByteArray &a, int pos, quint16 val)
{
        if(pos+1>=a.size()){
            qDebug()<<"CONVERT: Out of range";
            return;
        }
        auto ptr = reinterpret_cast<char*>(&val);
        a[pos+1] = ptr[0];
        a[pos] = ptr[1];
}

quint32 Convert::FB::readU24(const QByteArray &a, int pos)
{
    quint32 res = 0x00;
    if(pos+2>=a.size ()){
        qDebug()<<"CONVERT: Out of range";
        return 0x00;
    }

    res |= (a[pos] << 16  ) & 0xFF0000;
    res |= (a[pos+1] << 8 ) & 0x00FF00;
    res |=  a[pos+2]        & 0x0000FF;

    return res;
}

qint32 Convert::FB::readI24(QByteArray &a, int pos)
{

    if((pos+2)>=a.size()){
        qDebug()<<"CONVERT: Out of range";
        return 0x00;
    }

    qint32 res;
    if (a[pos] & 0x80) {
        res = 0xff << 24;
        res |= (a[pos+0] << 16)&0x00FF0000;
        res |= (a[pos+1] << 8)&0x0000FF00;
        res |= (a[pos+2] << 0)&0x000000FF;

        return res;//(0xff << 24) | (a[pos+0] << 16) | (a[pos+1] << 8) | (a[pos+2] << 0);
    } else{
        res  = (a[pos+0] << 16)&0x00FF0000;
        res |= (a[pos+1] << 8)&0x0000FF00;
        res |= (a[pos+2] << 0)&0x000000FF;
    }
    return res;
    return (a[pos+0] << 16) | (a[pos+1] << 8) | (a[pos+2] << 0);
}

void Convert::FB::writeU24(QByteArray &a, int pos, quint32 val)
{
    if(pos+2>=a.size()){
        qDebug()<<"CONVERT: Out of range";
        return;
    }
    auto ptr = reinterpret_cast<char*>(&val);
    a[pos]   = ptr[2];
    a[pos+1] = ptr[1];
    a[pos+2] = ptr[0];
}

qint32 Convert::FB::readI32(const QByteArray &a, int pos)
{
    quint32 res = 0x00;
    if(pos+3>=a.size()) {
        qDebug()<<"CONVERT: Out of range";
        return 0x00;
    }

    res |= (a[pos] << 24  )   & 0xFF000000;
    res |= (a[pos+1] << 16  ) & 0x00FF0000;
    res |= (a[pos+2] << 8 )   & 0x0000FF00;
    res |=  a[pos+3]          & 0x000000FF;

    return res;
}

quint32 Convert::FB::readU32(const QByteArray &a, int pos)
{
    quint32 res = 0x00;
    if(pos+3>=a.size()) {
        qDebug()<<"CONVERT: Out of range";
        return 0x00;
    }

    res |= (a[pos] << 24  )   & 0xFF000000;
    res |= (a[pos+1] << 16  ) & 0x00FF0000;
    res |= (a[pos+2] << 8 )   & 0x0000FF00;
    res |=  a[pos+3]          & 0x000000FF;

    return res;
}

void Convert::FB::writeI32(QByteArray &a, int pos, qint32 val)
{
    if(pos+3>a.size()){
        qDebug()<<"CONVERT: Out of range";
        return;
    }
    auto ptr = reinterpret_cast<char*>(&val);
    a[pos]   = ptr[3];
    a[pos+1] = ptr[2];
    a[pos+2] = ptr[1];
    a[pos+3] = ptr[0];
}

//
//////////////////// FL
//

quint8 Convert::FL::readU8(const QByteArray &a, int &pos)
{
    quint8 value = 0;

    if (pos >= a.size()) {
        qDebug()<<"CONVERT: Out of range";
        return 0x00;
    }

    value = a[pos++];
    return value;
}

void Convert::FL::writeU8(QByteArray &a, int pos, quint8 val)
{
    if (pos>=a.size()) {
        qDebug()<<"CONVERT: Out of range";
        return;
    }

    a[pos] = val;
}

quint16 Convert::FL::readU16(const QByteArray &a, int &pos)
{
    quint16 value = 0;

    if ((pos + 1) >= a.size()){
        qDebug()<<"CONVERT: Out of range";
        return 0x00;
    }


    value |= (a[pos+1] << 8 ) & 0xFF00;
    value |= (a[pos]        )& 0x00FF;

    pos += 2;
    return value;
}

void Convert::FL::writeU16(QByteArray &a, int pos, quint16 val)
{
    if (pos+1>=a.size()) {
        qDebug()<<"CONVERT: Out of range";
        return;
    }
    auto ptr = reinterpret_cast<char*>(&val);
    a[pos+1] = ptr[1];
    a[pos] = ptr[0];
}

quint32 Convert::FL::readU24(const QByteArray &a, int &pos)
{
    quint32 value = 0;

    if ((pos + 2) >= a.size()) {
        qDebug()<<"CONVERT: Out of range";
        return 0x00;
    }

    value |= ( a[pos+2] << 16  ) & 0xFF0000;
    value |= ( a[pos+1] << 8   ) & 0x00FF00;
    value |= ( a[pos]          ) & 0x0000FF;

    pos += 3;
    return value;
}

void Convert::FL::writeU24(QByteArray &a, int pos, quint32 val)
{
    if(pos+2>=a.size()){
        qDebug()<<"CONVERT: Out of range";
        return;
    }
    auto ptr = reinterpret_cast<char*>(&val);
    a[pos+2]   = ptr[2];
    a[pos+1] = ptr[1];
    a[pos] = ptr[0];

}

qint32 Convert::FL::readI32(const QByteArray &a, int &pos)
{
    qint32 value = 0x00;

    if(pos+3>=a.size()){
        qDebug()<<"CONVERT: Out of range";
        return 0x00;
    }

    value |= ( a[pos+3] << 24  )   & 0xFF000000;
    value |= ( a[pos+2] << 16  )   & 0x00FF0000;
    value |= ( a[pos+1] << 8   )   & 0x0000FF00;
    value |= ( a[pos]          )   & 0x000000FF;

    pos += 4;
    return value;
}

void Convert::FL::writeI32(QByteArray &a, int pos, qint32 val)
{
    if(pos+3>=a.size()){
        qDebug()<<"CONVERT: Out of range";
        return;
    }
    auto ptr = reinterpret_cast<char*>(&val);
    a[pos]   = ptr[0];
    a[pos+1] = ptr[1];
    a[pos+2] = ptr[2];
    a[pos+3] = ptr[3];
}

void Convert::setBit(quint8 * byte, int pos, bool value)
{
    if(!value){
        *byte &= ~(1 << pos);
    }
    else{
        *byte |= (1 << pos);
    }

//    *byte &= ~(1 << pos);
//    *byte &= ~(~b << pos);
}
