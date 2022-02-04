#ifndef STATE_H
#define STATE_H

#include <QObject>
#include <kmacro.h>
#include <QDebug>
#include "../serverinterface_global.h"

//!
//! \brief The StateData struct
//!
struct SERVERINTERFACE_LIBRARY_EXPORT StateData {
    int stateID;
    QString stationName;
    QVariant value;
    int socketDescriptor;
    QString resourceName;
    QString clientName;

    //!
    //! \brief Overloaded << operator used fro debug
    //! \param debug
    //! \param c
    //! \return QDebug object
    //!
    friend QDebug operator<<(QDebug debug, const StateData &c)
    {
        QDebugStateSaver saver(debug);
        debug << "(stateID: " << c.stateID << ", value changes:" << c.value << ')' << c.clientName;

        return debug;
    }

//    StateData(const StateData & data) : stateID(data.stateID), description(data.description), value(data.value) {}
};

Q_DECLARE_METATYPE(StateData)

class Station;

//!
//! \brief The Resource class
//!
class SERVERINTERFACE_LIBRARY_EXPORT Resource : public QObject
{
    Q_OBJECT
    K_READONLY_PROPERTY(QString, station, station, setStation, stationChanged, "")
    K_AUTO_PROPERTY(QString, name, name, setName, nameChanged, "")
    K_READONLY_PROPERTY(QString, client, client, setClient, clientChanged, "")
    K_READONLY_PROPERTY(QString, action, action, setAction, actionChanged, "")
    K_AUTO_PROPERTY(int, stateId, stateId, setStateId, stateIdChanged, 0)
    K_AUTO_PROPERTY(QVariant, value, value, setValue, valueChanged, QVariant())
    K_AUTO_PROPERTY(QString, controlType, controlType, setControlType, controlTypeChanged, QString())
    Station * stn = nullptr;

    friend Station;
public:
    explicit Resource(QObject * parent = nullptr);
    void setStn(Station *value);

public slots:
    void onPropertyChanged(QString name, QVariant prop);
protected:
//    K_AUTO_PROPERTY(KFlexibleModel*, propertyModel, propertyModel, setPropertyModel, propertyModelChanged, new KFlexibleModel(this))
};

#endif // STATE_H
