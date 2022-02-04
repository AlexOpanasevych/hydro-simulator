#ifndef ABSTRACTENGINE_H
#define ABSTRACTENGINE_H

#include <vector>
#include <map>
#include <functional>
#include "abstractpicontroller.h"
#include <messagedispatcher.h>
#include <sampledatacontainer.h>
#include <engineinterface.h>
#include <core_global.h>
#include <mutex>

class WaveSampleContainer;
class AbstractPIController;
class EngineInterface;

template <typename Callback, typename ... Args>
struct VariadicCallback
{
    VariadicCallback(Callback f) {
        registerCallback(f);
    }
    std::function<void(Args ...)> callbackFn;

    void registerCallback(Callback f) {

        this->callbackFn = [&](Args ... args) { f(args...); };
    }

    void operator ()(Args ... args){callbackFn(args...);}

};

//!
//! \brief The AbstractEngine class
//!
class CORE_LIBRARY_EXPORT AbstractEngine
{
public:
    AbstractEngine();
    ~AbstractEngine();

    //!
    //! \brief Get physical item sample data
    //! \return sample data container
    //!
    SampleDataContainer *getPiSamples() const;
//    WaveSampleContainer *getWaveSamples() const;
    EngineInterface *getInterface() const;
    //!
    //! \brief Function that starts the engine
    //!
    //!
    virtual void start() = 0;
    //!
    //! \brief Stops the engine and calculations
    //!
    virtual void stop() = 0;

//    int lastCreatedPIID;
    //!
    //! \brief getControllerSamples
    //! \return container with controller sample data
    //!
    SampleDataContainer *getControllerSamples() const;
//    unsigned long long engineTime() const;
    std::vector<std::function<void()>> oneSecondCallbacks;

    std::map<int, AbstractPIController *> &getPiControllers();
    QAtomicInteger<unsigned long long> m_engineTime = 0;
    bool doMath;


protected:
    //!
    //! \brief Function that proceeds engine and executes in separate thread
    //! \param uSecond
    //! \param dt
    //!
    void proceed(int uSecond, int dt);
    //!
    //! \brief Removes controller by \a id
    //! \param id
    //! \return \a true if delete is complete or \a false if not
    //!
    bool removeController(int id);
    SampleDataContainer *piSamples;
    //SamplaDataContainer *controllerSamples;
    //WaveSampleDataContainer *waveSamples;

    MessageDispatcher * messageDispatcher;
    EngineInterface *m_interface;
    //!
    //! \brief Inserts new controller
    //! \param id
    //! \param c
    //!
    void insertController(int id, AbstractPIController *c);
    //!
    //! \brief Proceeds msg sended to particular physical item controller
    //! \param header
    //! \param s
    //! \return
    //!
    QByteArray proceedItemMsg(Header header, QDataStream &s);
    std::map<int, AbstractPIController*> m_piControllers;
//    unsigned long long m_engineTime = 0;


private:
    int prevTime_us = 0;
//    static std::mutex m;
};


#endif // ABSTRACTENGINE_H
