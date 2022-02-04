#ifndef KADDITIONAL_H
#define KADDITIONAL_H

#include <kmacro.h>
#include <QString>

class KLIBCORELITE_EXPORT KAdditional {
public:
#ifdef __WIN32__
    static constexpr char separator = '\\';
    static inline const QString separatorString = "\\";
#else
    static constexpr char separator = '/';
    static inline const QString separatorString = "/";
#endif
    static QString constrainPath(const QString &path);
    static QString absolutePath(const QString &path);
    static bool urlEndsWith(const QUrl &url, const QString string);

    template<typename T>
    static QString numberToFixedSizeString(T number) {
        int size;
        switch (sizeof (number)) {
        case 1: size = 3; break;
        case 2: size = 5; break;
        case 4: size = 10; break;
        default: size = 16; break;
        }
        auto numString = QString::number(number);
        return numString + QString(' ').repeated(size - numString.size());
    }

    template<typename T>
    static std::string numberToFixedSizeStdString(T number) {
        return numberToFixedSizeString(number).toStdString();
    }

    template<typename T>
    static QList<T*> fillQObjectList(QObject *parent, int count);
};

template<typename T>
QList<T*> KAdditional::fillQObjectList(QObject *parent, int count) {
    QList<T*> result;
    for(int i = 0; i < count; ++i) {
        result.push_back(new T(parent));
    }
    return result;
}

class KLIBCORELITE_EXPORT KAdditionalPrivate : public QObject {
    Q_OBJECT
    K_PRIVATE_SINGLETON(KAdditionalPrivate)
    K_QML_SINGLETON_ALIAS(KAdditionalPrivate, KAdditional)
    KAdditionalPrivate() {}
public slots:
    inline QString absolutePath(const QString &path) { return KAdditional::absolutePath(path); }
    inline QString constrainPath(const QString &path) { return KAdditional::constrainPath(path); }
    inline bool urlEndsWith(const QUrl &url, const QString string) { return KAdditional::urlEndsWith(url, string); };
    inline QString urlPath(const QUrl &url) { return url.path(); };
};


#endif // KADDITIONAL_H
