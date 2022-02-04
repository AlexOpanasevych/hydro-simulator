#include "transmitter.h"
#include <streamconnector.h>
#include <QRandomGenerator>
#include <QByteArray>
Transmitter::Transmitter(QObject * parent) : AbstractSubsystem(parent)
{
//    QObject::connect(&timer, &QTimer::timeout, this, [](){
//        static int i = 0;
//        qDebug() << "---------------";
//        QVariantMap data;
//        QVector<double> sweep;

//        if( i++ % 4 ) {

//            for(int j = 0; j < 16; j++) {
//                if(j == 8) {
//                    sweep.push_back(1);
//                } else sweep.push_back(QRandomGenerator::global()->generateDouble());
//            }

//        } else {
//            for(int j = 0; j < 16; j++) {
//                sweep.push_back(QRandomGenerator::global()->generateDouble());
//            }

//        }
//        QVariantList list;
//        for(auto && sw : sweep) {
//            list.push_back(sw);
//        }
//        data["sweep"] = list;
//        StreamConnector::instance()->sendData(data, "dopler");
//    });
//    timer.start(200);
//    QObject::connect(&ep1sweep, &QTimer::timeout, this, [](){
//        static int i = 0;
//        qDebug() << "---------------";
//        QVariantMap data;
//        QVector<double> sweep;

//        if( i++ % 4 ) {

//            for(int j = 0; j < 16; j++) {
//                if(j == 8) {
//                    sweep.push_back(1);
//                } else sweep.push_back(QRandomGenerator::global()->generateDouble());
//            }

//        } else {
//            for(int j = 0; j < 16; j++) {
//                sweep.push_back(QRandomGenerator::global()->generateDouble());
//            }

//        }
//        QVariantList list;
//        for(auto && sw : sweep) {
//            list.push_back(sw);
//        }
//        data["sweep"] = list;
//        StreamConnector::instance()->sendData(data, "ep1sweep");
//    });
//    ep1sweep.start(200);

//    StreamConnector::instance()->addStreamSender("platinaSound");

//    connect(m->audioOutput(), &QAudioOutput::notify, this, [this](){
//    //        static int i = 0;
////            qDebug() << "---------------";
//            QVariantMap data;
//    //        QVector<double> sweep;

//    //        if( i++ % 4 ) {

//    //            for(int j = 0; j < 16; j++) {
//    //                if(j == 8) {
//    //                    sweep.push_back(1);
//    //                } else sweep.push_back(QRandomGenerator::global()->generateDouble());
//    //            }

//    //        } else {
//    //            for(int j = 0; j < 16; j++) {
//    //                sweep.push_back(QRandomGenerator::global()->generateDouble());
//    //            }

//    //        }
//    //        QVariantList list;
//    //        for(auto && sw : sweep) {
//    //            list.push_back(sw);
//    //        }
//    //        data["sweep"] = list;
////            qDebug() << "adadadadadaadadadadad";
//            if(m->device() != nullptr) {
//                data["sound"] = m->device()->buffer();
//                StreamConnector::instance()->sendData(data, "platinaSound");
//            }
//    });
//    platinaSound.start(200);
}

void Transmitter::proceed(KObservableList<Resource> &stateList, const StateData &data)
{
//    Q_UNUSED(stateList)
    Q_UNUSED(data)

    for(auto && state : stateList) {

    }

}

int Transmitter::order()
{
    return 0;
}

Transmitter::~Transmitter()
{
//    delete m;
}
