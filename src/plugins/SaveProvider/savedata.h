#ifndef SAVEDATA_H
#define SAVEDATA_H

#include <QObject>
#include <kmacro.h>
#include <QImage>

class SaveManager;
class SaveData : public QObject
{
    Q_OBJECT
    K_READONLY_PROPERTY(QVariant, saveData, saveData, setSaveData, saveDataChanged, QVariant())
    K_AUTO_PROPERTY(QString, saveName, saveName, setSaveName, saveNameChanged, "")
    K_AUTO_PROPERTY(QString, description, description, setDescription, descriptionChanged, "")
    K_READONLY_PROPERTY(QString, date, date, setDate, dateChanged, "")
    K_AUTO_PROPERTY(int, idx, idx, setIdx, idxChanged, 0)
    friend class SaveManager;
public:
    explicit SaveData(QObject *parent = nullptr) : QObject (parent) {}
    QVariant serialize();
    void deserialize(QVariant data);
};

#endif // SAVEDATA_H
