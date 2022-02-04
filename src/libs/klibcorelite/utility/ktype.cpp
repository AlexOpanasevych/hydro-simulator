#include "ktype.h"

#include <QCryptographicHash>
#include <QDataStream>

qint64 KTypeHashArgorithm::hash64(const QString &string) {
    QByteArray hash = QCryptographicHash::hash(QByteArray::fromRawData((const char*)string.utf16(), string.length()*2), QCryptographicHash::Md5);
    Q_ASSERT(hash.size() == 16);
    QDataStream stream(&hash, QIODevice::ReadOnly);
    qint64 a, b;
    stream >> a >> b;
    return a ^ b;
}



