#include "modelmanager.h"
#include <QDebug>
#include <kadditional.h>

ModelManager::ModelManager(QObject *parent): QObject(parent)
{
}

KFlexibleModel *ModelManager::getModel(QString id, QVariantList roles)
{
    KFlexibleModel *result {};

    if (m_models[id])
    {
        result = m_models[id];
        if (roles.length() > 0)
        {
            result->addRoles(roles);
        }
    }
    else
    {
        result = new KFlexibleModel(roles);
        result->setParent(this);
        m_models.insert(id, result);
    }

    return result;
}

void ModelManager::clearModel(QString id)
{
    KFlexibleModel *result {};
    if (m_models[id])
    {
        result = m_models[id];
        result->removeAll();
    }
}

bool ModelManager::contains(const QString id) const
{
    return m_models.contains(id);
}

