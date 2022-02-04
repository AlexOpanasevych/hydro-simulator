#include "kserialbusthreaded.h"

/*!
    \brief Constructor KSerialBusThreaded
*/
KSerialBusThreaded::KSerialBusThreaded()
{
    serialPort = new QSerialPort();
}

/*!
    \brief KSerialBusThreaded destructor. Wait while thread stopped and close serial port
    \details You must call destructor in your application for correct close program
*/
KSerialBusThreaded::~KSerialBusThreaded()
{
    stopThread();

    if (serialPort->isOpen()) {
        VDebug::Info("Close serial port");
        serialPort->close();
    }
}

/*!
    \brief Return status of QSerialPort connection
    \return Is the KSerialBusThreaded connected to the target COM-port
*/
bool KSerialBusThreaded::isConnected()
{
    return serialPort->isOpen();
}

/*!
    \brief Return status of transceive circular buffer
    \return Is the buffer empty. True = buffer is empty
*/
bool KSerialBusThreaded::isEmptyTX()
{
     return m_transmitBuf.bufIsEmpty();
}

/*!
    \brief Return status of receive circular buffer
    \return Is the buffer empty. True = buffer is empty
*/
bool KSerialBusThreaded::isEmptyRX()
{
    return m_receiveBuf.bufIsEmpty();
}

/*!
    \brief Add data to transceiver circular buffer
    \param[in] QByteArray object with data which need to be transieved
*/
void KSerialBusThreaded::push(const QByteArray &data)
{
    m_transmitBuf.push(data);
}

/*!
    \brief Get readed data from receive circular buffer
    \return QByteArray with readed data
*/
QByteArray KSerialBusThreaded::pop()
{
    if (!serialPort->isOpen()) {
        VDebug::Warning("Serial port not open");
        return QByteArray();
    }

    bool ok;
    auto data = m_receiveBuf.pop(&ok);
    return data;
}

/*!
    \brief Stopping main thread
    \details Wait for function 'run' stopped
*/
void KSerialBusThreaded::stopThread()
{
    stopFlag = true;
    m_runTask.waitForFinished();
}

/*!
    \brief Set main port params
    \param[in] name COM-port name
    \param[in] baudRate COM-port speed
    \return Status of applying settings. False = settings not applying
    \details If COM-port is open the settings not applying
*/
bool KSerialBusThreaded::setPort(QString name, uint64_t baudRate, QSerialPort::Parity parity) {
    if (serialPort != nullptr) {
        if (serialPort->isOpen()) {
            qDebug() << "Serial port is open. New settings not apply";
            return false;
        }

        serialPort->setPortName(name);
        serialPort->setBaudRate(baudRate);
        serialPort->setParity(parity);
        return true;
    }
    return false;
}

/*!
    \brief Get parameters for automatic generating form
    \return list of parameters
    \details See struct KBus::ConnectionData. Name - title of element, type - type of element, data - data of element
*/
QList<KBus::ConnectionData> KSerialBusThreaded::getConnectionData()
{
    QStringList busPortsValues;
    auto info = QSerialPortInfo::availablePorts();
    for (auto &it: info) {
        busPortsValues.push_back(it.portName());
    }

    QList<ConnectionData> cd;
    cd.push_back({ "KSerialBusThreaded",  "MainLabel", "" });
    cd.push_back({"Main features", "Label", ""});
    cd.push_back({ "Port",  "ComboBox", busPortsValues});
    cd.push_back({ "Speed", "ComboBox", QStringList { "9600", "1000000", "115200"}});
    cd.push_back({ "Parity", "ComboBox", QStringList {"NoParity", "EvenParity", "OddParity"}});
    return cd;
}

/*!
    \brief Start thread and begin work: open serial, execution read/write operations, calling workers
*/
void KSerialBusThreaded::startOverride()
{
    m_runTask = QtConcurrent::run(this, &KSerialBusThreaded::run);
}

void KSerialBusThreaded::logTime(const QByteArray &data)
{
    static auto t0 = std::chrono::high_resolution_clock::now();
    auto t1 = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0).count() / 1000.;
    t0 = t1;
    qDebug() << "elapsed: " << elapsed << "; size: " << data.size() << data.toHex();
}

/*!
    \brief Callback for receive packets
    \details Proceed all workers
*/
void KSerialBusThreaded::onPacketReceived(const QByteArray & data)
{
    qDebug() << "REC:" << data.toHex('.');
    auto foo = m_filterCallback;
    if (foo != nullptr) if (foo(data)) return;

    bool workerTakePacket = false;
    for (auto w: m_workers) {
        if(w->proceedPacket(data, timer.elapsed())) {
            workerTakePacket = true;
            break;
        }
    }

    if (workerTakePacket == false) {
        m_receiveBuf.push(data);
    }
}

/*!
    \brief Main KSerialBusThreaded loop
*/
void KSerialBusThreaded::run()
{
    VDebug::Info("KSerialBusThreaded run start");
    QThread::currentThread()->setPriority(QThread::TimeCriticalPriority);
    runThread = QThread::currentThread();

    if (!serialPort->open(QSerialPort::ReadWrite)) {
        VDebug::Error("Fail to open COM-port");
        return;
    }
    VDebug::Success("Port successfully open");

    serialPort->clear();

    com.setReceivePackCallback(std::bind(&KSerialBusThreaded::onPacketReceived, this, std::placeholders::_1));
    com.setTransmitPackCallback([this](QByteArray ar) {
        serialPort->write(ar);
        serialPort->waitForBytesWritten(1);
    });

    timer.start();
    bool transmitCmdExist;
    QByteArray readedSerialData;

    while (!stopFlag) {
        //QThread::msleep(1);

        serialPort->waitForReadyRead(1);
        readedSerialData = serialPort->readAll();
        if(readedSerialData.size() > 0){
            com.proceedRawData(readedSerialData);
        }

        auto&& pack = m_transmitBuf.pop(&transmitCmdExist);
        if (transmitCmdExist) {
            com.send(pack);
        }

        for (auto w: m_workers) {
            auto&& packs = w->getPacketsForSend(timer.elapsed());
            for (auto&& p : packs) {
                com.send(p);
            }
        }
    }

    VDebug::Info("Close serial port");
    serialPort->close();
    delete serialPort;

    VDebug::Info("KSerialBusThreaded run end");
}
