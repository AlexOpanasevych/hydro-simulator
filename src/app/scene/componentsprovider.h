#ifndef COMPONENTSPROVIDER_H
#define COMPONENTSPROVIDER_H

#include <QObject>
#include <QQmlComponent>
#include <QQmlEngine>

//!
//! \brief The ComponentsProvider class using only for internal qml files (non plugin) to provides potentional memory leak
//! Used only in method KScene::getCompontnts
//!
class ComponentsProvider : public QObject{
    Q_GADGET
public:
    ComponentsProvider(QObject *parent = nullptr) : QObject(parent) {}
    QQmlComponent *createComponent(QUrl url);
    void setEngine(QQmlEngine *engine) { m_engine = engine; }

private:
    QHash<QUrl, QQmlComponent*> m_componentsHash;
    QQmlEngine *m_engine = nullptr;
};

#endif // COMPONENTSPROVIDER_H
