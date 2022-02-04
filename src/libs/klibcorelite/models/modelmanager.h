#ifndef MODELMANAGER_H
#define MODELMANAGER_H

#include <QObject>
#include <kmacro.h>
#include <models/kflexiblemodel.h>

class KLIBCORELITE_EXPORT ModelManager: public QObject
{
    Q_OBJECT
    K_SINGLETON(ModelManager)
    K_QML_SINGLETON(ModelManager)
    explicit ModelManager(QObject *parent = nullptr);

private:
    QMap<QString, KFlexibleModel *> m_models;

public slots:
    KFlexibleModel * getModel(QString id, QVariantList roles = QVariantList());
    void clearModel(QString id);
    bool contains(const QString id) const;
};

#endif // MODELMANAGER_H
