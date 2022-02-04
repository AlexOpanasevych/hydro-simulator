#ifndef CLIPBOARD_H
#define CLIPBOARD_H
#include <QObject>
#include "kmacro.h"
#include <QGuiApplication>
#include <QClipboard>
class ClipBoard : public QObject
{
    Q_OBJECT
    K_SINGLETON(ClipBoard)
    K_QML_SINGLETON(ClipBoard)
public:
    ClipBoard(QObject * parent = nullptr);
public slots:
    void setText(QString text);
private:
    QClipboard* clipboard;
};
#endif // CLIPBOARD_H
