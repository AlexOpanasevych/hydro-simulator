#ifndef SAVEMANAGER_H
#define SAVEMANAGER_H

#include <QObject>
#include <utility/kobservablelist.h>
#include <kmacro.h>
#include <savedata.h>
#include <base/sceneprivateaccessor.h>

class SaveManager : public QObject
{
    Q_OBJECT
    K_CONST_PROPERTY(ScenePrivateAccessor*, sceneAccess, new ScenePrivateAccessor(this))
    K_READONLY_PROPERTY(KObservableModel *, saves, saves, setSaves, savesChanged, nullptr)
    K_AUTO_PROPERTY(SaveData *, currentData, currentData, setCurrentData, currentDataChanged, nullptr)
    K_AUTO_PROPERTY(SaveData *, cacheData, cacheData, setCacheData, cacheDatachanged, nullptr)
    K_READONLY_PROPERTY(QVariantList, namesList, namesList, setNamesList, namesListnChanged, QVariantList())
public:
    explicit SaveManager(QObject *parent = nullptr);
    ~SaveManager();
    enum Mode{
        LOOKING,
        EDITING,
        CREATING,
        REPLACING
    };
    Q_ENUM(Mode)
public slots:
    void load(SaveData *data);
    void save(SaveData *data);
    void cancelCreating();
    void cancelReplacing();
    void cancelEditing();
    void clone(SaveData *data);
    void edit(SaveData *data);
    void replace(SaveData *old, SaveData *newOne);
    void updateCacheData();
    void prepareReplacing();
    void removeCurrentData();
    SaveData * createSaveData();

    bool checkName(QString name);

private:
    K_READONLY_PROPERTY(Mode, mode, mode, setMode, modeChanged, Mode::LOOKING)
    KObservableList<SaveData> m_savesList;
    void saveConfig();
    void loadConfig();
};

#endif // SAVEMANAGER_H
