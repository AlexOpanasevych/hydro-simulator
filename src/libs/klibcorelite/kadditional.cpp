#include "kadditional.h"

#include <QCoreApplication>
#include <QStack>

QString KAdditional::constrainPath(const QString &path) {
    if(path.size() <= 0)
        return "";

    auto results = path.split(separator);

    QStack<QString> stack;
    for(auto&& r : results) {
        if(r == "..") {
            stack.pop_back();
        } else if(r == ".") {

        } else if(r == "") {

        } else {
            stack.push_back(r);
        }
    }

    QString result = (path[0] == separator) ? separatorString : "";
    int i = 0;
    for(auto&& s : stack) {
        result += ((i == 0) ? "" : separatorString) + s;
        i++;
    }
    return result;
}

QString KAdditional::absolutePath(const QString &path) {
    if(QCoreApplication::arguments().size() <= 0) return "";
    if(path.size() <= 0) return "";

#ifdef __WIN32__
    if(path.size() > 2 && path[1] == ":" && path[2] == separator) return path;
#else
    if(path[0] == separatorString) return path;
#endif

    return constrainPath(QCoreApplication::arguments()[0] + separatorString + ".." + separatorString + path);
}

bool KAdditional::urlEndsWith(const QUrl &url, const QString string) {
    return url.path().endsWith(string);
}
