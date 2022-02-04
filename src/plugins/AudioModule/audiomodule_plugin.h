#ifndef AUDIOMODULE_PLUGIN_H
#define AUDIOMODULE_PLUGIN_H

#include "audiomodule.h"

#include <QQmlExtensionPlugin>
#include <AbstractPluginInterface.h>
#include <AbstractServerInterface.h>

class AudioModulePlugin : public QQmlExtensionPlugin, public AbstractPluginInterface
{
    Q_OBJECT
    Q_INTERFACES(AbstractPluginInterface)
    Q_PLUGIN_METADATA(IID QQmlExtensionInterface_iid)

    //!
    //! \brief Gets QML components paths
    //! \return components map of QML components paths
    //!
    ComponentsMap getComponents() override;

    //!
    //! \brief Init plugin
    //! \param engine
    //!
    void init(AbstractEngine *engine, QString appPath = "", const QVariantList &depencies = QVariantList()) override;

    AbstractEngine * engine = nullptr;
//    QSharedPointer<AudioModule> module {new AudioModule(this)};
    AbstractServerInterface * m_serverInterface;


    QTimer * sendTimer = new QTimer(this);

public:
    //!
    //! \brief Register QML types
    //! \param uri
    //!
    void registerTypes(const char *uri) override;
    ~AudioModulePlugin() {delete sendTimer;}
    std::function<void(QVariantMap)> f;
public slots:
    //!
    //! \brief Adds new sound to sound module
    //! \param id
    //! \param soundName
    //! \param data
    //!
    void addSound(int id, QString soundName, QVariantMap data);

    //!
    //! \brief Stops all sounds or specific sound
    //! \param id
    //! \param soundName
    //!
    void stopSound(int id = -1, QString soundName = "");

private:
    QString m_appPath = "";
};

#endif // AUDIOMODULE_PLUGIN_H
