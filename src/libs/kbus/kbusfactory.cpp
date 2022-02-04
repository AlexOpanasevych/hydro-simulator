#include "kbusfactory.h"
#include "buses/kserialbusthreaded.h"

/*!
    \brief Once create object of KBusFactory and install factory products
    \return Pointer to KBusFactory object
*/
KBusFactory *KBusFactory::instance() {
    static KBusFactory* inst = nullptr;
    if (inst == nullptr) {
        inst = new KBusFactory();
        inst->installDefaultCreators();
    }
    return inst;
}

/*!
    \brief Push new KBusFactory product to storage
    \param[in] type - name of KBus implementation
    \param[in] creator - function which create selected object, initializes it with parameters and return pointer to it
    \param[in] getData - pointer to static function if KBus implementation which return list of struct ConnectionData
    \details Provides a simple way to insert new KBus implementation to m_creators.
             ConnectionData used for automatic create GUI forms to select parameters
*/
void KBusFactory::instalBusCreator(QString type, std::function<KBus *(QVariantMap)> creator,
                                   std::function<QList<KBus::ConnectionData>()> getConnectionData)
{
    m_creators.insert(type, { .creator = creator, .getConnectionData = getConnectionData });
}

/*!
    \brief Create instance of KBusFactory product
    \param[in] name - name of KBus implementation
    \param[in] params - parameters for initialization KBusFactory product
    \details The created product must exist in Factory, previously added by instalBusCreator()
    \return Pointer to selected KBusFactory product
*/
KBus *KBusFactory::create(QString name, QVariantMap params)
{
    auto s = m_creators.find(name);
    if (s != m_creators.end()) {
        return s->creator(params);
    }
    VDebug::Error("Selected KBus type does not exist");
    return nullptr;
}

/*!
    \brief Added exists KBusFactory products to storage when instance() first called
*/
void KBusFactory::installDefaultCreators()
{
    instalBusCreator("KSerialBusThreaded", [this](auto params) {
        auto bus = new KSerialBusThreaded();
        bus->setPort(params["Port"].toString(), params["Speed"].toInt(), parityMap[params["Parity"].toString()]);
        return bus;
    }, &KSerialBusThreaded::getConnectionData);
}

/*!
    \brief Get names of all exist KBusFactory products
    \return List of names
*/
QList<QString> KBusFactory::getCreatorsNames()
{
    QList<QString> names;
    for (auto it = m_creators.begin(); it != m_creators.end(); ++it) {
        names.push_back(it.key());
    }
    return names;
}

/*!
    \brief Get createData of selected KBusFactory product
    \param[in] name - name of KBus implementation
    \return List of ConnectionData
*/
QList<KBus::ConnectionData> KBusFactory::getBusCreateData(QString name)
{
    auto el = m_creators.find(name);
    if (el != m_creators.end()) {
        KBusFactory::CreateInfo creator = el.value();
        return creator.getConnectionData();
    }

    VDebug::Warning("getBusCreateData not find the creator");
    return QList<KBus::ConnectionData>();
}
