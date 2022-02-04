#include "clipboard.h"
ClipBoard::ClipBoard(QObject* parent) : QObject(parent)
{
    clipboard = QGuiApplication::clipboard();
}
void ClipBoard::setText(QString text)
{
    clipboard->setText(text);
}
