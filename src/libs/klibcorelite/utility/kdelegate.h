#ifndef KDELEGATE_H
#define KDELEGATE_H

#include <QQuickItem>
#include <kmacro.h>

class KLIBCORELITE_EXPORT KDelegate : public QQuickItem {
    Q_OBJECT
    K_OBJECT
    K_QML_TYPE(KDelegate)
    K_AUTO_PROPERTY(QQmlComponent*, contentComponent, contentComponent, setContentComponent, contentComponentChanged, nullptr)
    K_AUTO_PROPERTY(QString, modelDataType, modelDataType, setModelDataType, modelDataTypeChanged, QString())

    Q_CLASSINFO("DefaultProperty", "contentComponent")
public:
    explicit KDelegate(QQuickItem *parent = nullptr);

signals:

};

#endif // KDELEGATE_H
