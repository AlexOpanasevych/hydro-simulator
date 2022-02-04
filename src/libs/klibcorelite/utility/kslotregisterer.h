#ifndef KSLOTREGISTERER_H
#define KSLOTREGISTERER_H

#include <QObject>
#include <QMetaMethod>
#include "../kmacro.h"
#include "../klibcorelite_global.h"

class KLIBCORELITE_EXPORT KSlotRegisterer : public QObject
{
    Q_OBJECT
    K_SINGLETON(KSlotRegisterer)
    KSlotRegisterer() {}
    enum class ReduceReason {ReduceByName};
    QMap<QString, std::function<void(const QString & , const QVariant &)>> map;
public:
//    template<typename Object1, typename Object2, typename Signal>
//    void registerSlot(const Object1 *obj1, Signal signal1, Object2 *);
    void registerSignal(const QObject *obj1, const char * signal1, QObject * obj2);
    virtual ~KSlotRegisterer() {}
};

//template<typename Object1, typename Object2, typename Signal>
//inline void KSlotRegisterer::registerSlot(const QObject *obj1, const  signal1, QObject* obj2)
//{
//    connect(obj1, signal1, obj2, [=](const QString & name, const QVariant & prop){

//        if(obj2 == nullptr) return;

//        const auto signal = "on" + (!name.isEmpty() ? name[0].toUpper() + name.mid(1) : QString()) + "Changed";
//        for (int i = 0; i < obj2->metaObject()->methodCount(); i++) {
//            auto m = obj2->metaObject()->method(i);
//            if(QString(m.name()).contains(signal)) {
//                m.invoke(obj2)
//                QMetaObject::invokeMethod(obj2, (signal).toUtf8().data(), Q_ARG(QVariant, prop));

//            }

//        }
//    });

//        connect(station(), &Station::propertyChanged, this, [&](const QString & name, const QVariant & prop){
//            QString upper = !name.isEmpty() ? name[0].toUpper() + name.mid(1) : "";
//            QString slotName = SLOT(onEpChanged());
//            qDebug(s) << slotName;
//            if(slotName.contains("on" + upper + "Changed"))
//    //            QMetaObject::invokeMethod(this, slot, Q_ARG(QVariant, prop));
//                onEpChanged(prop);
//        });
//}

inline void KSlotRegisterer::registerSignal(const QObject *obj1, const char * signal1, QObject * obj2)
{
//    switch (reason) {
//    case ReduceReason::ReduceByName:
//        connect(obj1, signal1, obj2, [=](QString name, QVariant prop){
//            if(QString(signal2).contains(name + "Changed"))
//                QMetaObject::invokeMethod(obj2, signal2, Q_ARG(QVariant, prop));
//        });
//        break;
//    }
}

#endif // KSLOTREGISTERER_H
