#ifndef CLIENTSIDEPARSER_H
#define CLIENTSIDEPARSER_H

#include <QVariantMap>
#include <idataparser.h>
#include <QFile>
#include "qzipreader_p.h"
#include <QDataStream>
#include <QDebug>
#include <QJsonDocument>

#define QML_ZIP_FILE uint8_t(2)
#define QML_AS_STRING uint8_t(1)


class ClientSideParser : public IDataParser
{
    QByteArray data;
    QVariantList result;
    int id;
    uint8_t typeOfData;
public:
    ClientSideParser(uint8_t typeOfData);
    int parse(QDataStream *s) override;
    QVariantList captured() override;
    QMap<uint8_t, std::function<void(QDataStream *, QVariantList *)>> miniParsers =
    {{QML_ZIP_FILE, [](QDataStream *s, QVariantList * result){
        //const char * file_path = "qml.zip";
        QString dirname;
        *s >> dirname;
        QFile file(dirname);
        file.open(QFile::WriteOnly);
        QByteArray data;
        *s >> data;
        file.write(data);
        QZipReader zip(dirname);
        if(zip.exists()) {
            qDebug() << "file exists and not broken, number of items in archive:" << zip.count();
        }
        zip.extractAll(QLatin1String("./resources/"));
        zip.close();
        QVariantMap newMap;
        newMap.insert(dirname, QString("./resources"));
        result->push_back(newMap);
    }},
    {QML_AS_STRING, [](QDataStream *s, QVariantList * result){
        QJsonDocument data;
        QVariantMap inMap;
        *s >> data;

        inMap = data.toVariant().toMap();

//        for (int i = 0; i < mapSize; i++) {
//            QString key;
//            QVariant value;
//            (*s) >> key >> value;
//            inMap.insert(key, value);
//        }
//        qDebug() << inMap;
        result->push_back(inMap);
//        if(s->atEnd()) qDebug() << "end datastr";
    }
    }};
};

#endif // CLIENTSIDEPARSER_H

