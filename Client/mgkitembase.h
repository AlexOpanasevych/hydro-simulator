#ifndef MGKITEM_H
#define MGKITEM_H

#include <QQuickItem>
#include <kmacro.h>
#include <QMap>

class StateCommunicator;
class MGKItem;

class Resource : public QObject {
    Q_OBJECT
public:
    friend StateCommunicator;
    friend MGKItem;
    explicit Resource(QObject * parent = nullptr);
    ~Resource() {qDebug() << "deleted~~~~~~~~~~~~~!!!!!!!!!!!!";}
    K_AUTO_PROPERTY(int, stateId, stateId, setStateId, stateIdChanged, 0)
    K_READONLY_PROPERTY(QString, station, station, setStation, stationChanged,"")
    K_READONLY_PROPERTY(QString, name, name, setName, nameChanged,"")
protected:
    //K_READONLY_PROPERTY(QString, serializeType, serializeType, setSerializeType, serializeTypeChanged, 0)

};

class MGKItem : public QObject
{
    Q_OBJECT
    K_QML_TYPE(MGKItem)
    K_AUTO_PROPERTY(QVariant, requestValue, requestValue, setRequestValue, requestValueChanged, QVariant())
    K_AUTO_PROPERTY(int, globalIndex, globalIndex, setGlobalIndex, globalIndexChanged, 0)
    K_AUTO_PROPERTY(QString, caption, caption, setCaption, captionChanged, "")
    K_READONLY_PROPERTY(QString, controlType, controlType, setControlType, controlTypeChanged, "")
    K_AUTO_PROPERTY(QColor, color, color, setColor, colorChanged, "")
    K_AUTO_PROPERTY(QVariantMap, additionalQml, additionalQml, setAdditionalQml, additionalQmlChanged, {})
    K_AUTO_PROPERTY(int, x, x, setX, xChanged, 0)
    K_AUTO_PROPERTY(int, y, y, setY, yChanged, 0)
    K_AUTO_PROPERTY(int, width, width, setWidth, widthChanged, 0)
    K_AUTO_PROPERTY(int, height, height, setHeight, heightChanged, 0)
    K_AUTO_PROPERTY(QString, clientName, clientName, setClientName, clientNameChanged, "")
    Q_PROPERTY(QVariant value READ value WRITE setValue NOTIFY valueChanged)

//    K_AUTO_PROPERTY(QList<Resource*>, actions, actions, setActions, actionsChanged, {})
    K_AUTO_PROPERTY(Resource*, resource, resource, setResource, resourceChanged, nullptr)
    friend StateCommunicator;
public:
    MGKItem(QObject *parent = nullptr);

    void serialize(QDataStream *);
    void deserialize(QDataStream *);

    void init(QVariantMap prop);

    ~MGKItem();

    QVariant value();

signals:
    void valueChanged(QVariant);
public slots:
    void setValue(QVariant value);
private:
    QVariant m_value;
};

#endif // MGKITEMBASE_H
