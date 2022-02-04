#ifndef KABSTRACTFACTORY_H
#define KABSTRACTFACTORY_H


#include "ktype.h"

#include <map>
#include <functional>

template<typename KeyType, typename BaseClass>
class KAbstractFactory {
    typedef std::map<KeyType, std::function<BaseClass*()>> CreaterMap;

    CreaterMap creaters;
    std::map<BaseClass*, KeyType> typeNames;
public:
    KAbstractFactory();
    BaseClass *create(KeyType key);
    KeyType type(BaseClass *object);

    template<typename T>
    KeyType registerType();

    template<typename T>
    KeyType registerType(KeyType key);
};

template<typename KeyType, typename BaseClass>
KAbstractFactory<KeyType, BaseClass>::KAbstractFactory() {}

template<typename KeyType, typename BaseClass>
KeyType KAbstractFactory<KeyType, BaseClass>::type(BaseClass *object) {
    return typeNames.find(object) != typeNames.end() ? typeNames[object] : KeyType();
}

template<typename KeyType, typename BaseClass>
template<typename T>
KeyType KAbstractFactory<KeyType, BaseClass>::registerType() {
    return registerType<T>(KType<T>());
}

template<typename KeyType, typename BaseClass>
template<typename T>
KeyType KAbstractFactory<KeyType, BaseClass>::registerType(KeyType key) {
    creaters[key] = []() {
        return new T();
    };
    return key;
}

template<typename KeyType, typename BaseClass>
BaseClass *KAbstractFactory<KeyType, BaseClass>::create(KeyType key) {
    if(creaters.find(key) == creaters.end()) {
        printf("%s", (std::string(__PRETTY_FUNCTION__) + " unknown key.").c_str());
        return nullptr;
    }
    return creaters[key]();
}

#endif // KABSTRACTFACTORY_H
