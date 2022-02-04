#include "componentsprovider.h"

QQmlComponent *ComponentsProvider::createComponent(QUrl url)
{
    if (m_engine == nullptr) return nullptr;
    if (m_componentsHash.contains(url)){
        return m_componentsHash[url];
    }else{
        auto tmp = new QQmlComponent(m_engine, url, this);
        m_componentsHash.insert(url, tmp);
        return tmp;
    }
}
