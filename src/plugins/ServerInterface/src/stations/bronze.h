#ifndef BRONZE_H
#define BRONZE_H

#include <gui/PythonQtScriptingConsole.h>
#include <src/station.h>
#include <src/stations/bronzeSubsys/bronzereceiver.h>
#include <src/stations/bronzeSubsys/bronzetransmitter.h>


class BronzeEntity : public QObject {
    Q_OBJECT
    K_PROTECTED_PROPERTY(QString, name, name, setName, nameChanged, "")
    K_PROTECTED_PROPERTY(Resource *, resource, resource, setResource, resourceChanged, nullptr)
public:
        BronzeEntity(const QString & name, QObject * parent = nullptr);
    BronzeEntity(const QString & name, Resource * resource, QObject * parent = nullptr);
};

class BronzeButton : public BronzeEntity {
    Q_OBJECT
    K_AUTO_PROPERTY(int, led, led, setLed, ledChanged, 0)
public:
    BronzeButton(const QString & name, QObject * parent = nullptr);
    BronzeButton(const QString & name, Resource * resource, QObject * parent = nullptr);
};

class BronzeNumberView : public BronzeEntity {
    Q_OBJECT
    K_AUTO_PROPERTY(double, value, value, setValue, valueChanged, 0)
    public:
        BronzeNumberView(const QString & name, QObject * parent = nullptr);
        BronzeNumberView(const QString & name, Resource * resource, QObject * parent = nullptr);
};

class Bronze;

class MGK345Wrapper : public QObject{
Q_OBJECT
    Bronze * station = nullptr;

public:
    MGK345Wrapper(Bronze * station = nullptr);
public slots:
    BronzeButton * new_BronzeButton(const QString & name);

    BronzeNumberView * new_BronzeNumberView(const QString & name) {
        auto nv = new BronzeNumberView(name);
        connect(nv, &BronzeNumberView::valueChanged, this, [](double v){qDebug() << "nv value changed" << v;});
        return nv;
    }


    QVariant get(BronzeEntity * entity, const QString & propName);
    void set(BronzeEntity * entity, const QString & propName, const QVariant & value);

};



class Bronze : public Station
{
    Q_OBJECT
    K_QML_TYPE(Bronze)
    K_AUTO_PROPERTY(QString, appPath, appPath, setAppPath, appPathChanged, "")
    BronzeReceiver * receiver = nullptr;
    BronzeTransmitter * transmitter = nullptr;
    QString filePath = "/src/plugins/ServerInterface/src/subsystems/bronze/scripts/logic.py";
    PythonQtScriptingConsole * console = nullptr;
    PythonQtObjectPtr  mainContext;
public:
    friend MGK345Wrapper;
    explicit Bronze(QObject * parent = nullptr);
    template <typename T> QObject * creatorImpl() {return new T(this);}
    Q_INVOKABLE void updatePythonLogic();
public slots:

};

#endif // BRONZE_H
