#ifndef KTYPE_H
#define KTYPE_H


#include <string>
#include <cxxabi.h>
#include <QString>
#include <klibcorelite_global.h>

template <typename T>
class KLIBCORELITE_EXPORT KType {
    QString typeName;
    qint64 m_hash;
public:
    KType();
    operator QString() const;
    operator const char*() const;
    operator std::string() const;

    QString name() const;
    QString toString() const;

    qint64 hash() const;
};

class KLIBCORELITE_EXPORT KTypeHashArgorithm {
public:
    static qint64 hash64(const QString & string);
};

template<typename T>
KType<T>::KType() {
    std::string originalTypeName = typeid (T).name();
    size_t originalTypeNameSize = originalTypeName.size();
    int status = 0;
    char *demangleResult = abi::__cxa_demangle(originalTypeName.c_str(), nullptr, &originalTypeNameSize, &status);
    if(status == 0) {
        typeName = demangleResult;
    } else if(status == -1) {
        printf("KType: Memory allocation failiure occurred.");
    } else if(status == -2) {
        printf("KType: Mangled_name is not a valid name under the C++ ABI mangling rules.");
    } else if(status == -3) {
        printf("KType: One of the arguments is invalid.");
    }
    std::free(demangleResult);
    m_hash = KTypeHashArgorithm::hash64(typeName);
}

template<typename T>
QString KType<T>::toString() const {
    return typeName;
}

template<typename T>
KType<T>::operator const char*() const {
    return toString().toStdString().c_str();
}

template<typename T>
KType<T>::operator QString() const {
    return typeName;
}

template<typename T>
qint64 KType<T>::hash() const {
    return m_hash;
}

template<typename T>
QString KType<T>::name() const {
    return typeName;
}

template<typename T>
KType<T>::operator std::string() const {
    return typeName.toStdString();
}


#endif // KTYPE_H
