#include "kclassregistry.h"



QStringList kAvailableClasses() {
    return KClassRegistry::m_typeNames;
}

QObject *kInstantinate(QString className) {
    return KClassRegistry::factory.create(className);
}


