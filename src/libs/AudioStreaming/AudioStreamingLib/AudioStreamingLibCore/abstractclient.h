#ifndef ABSTRACTCLIENT_H
#define ABSTRACTCLIENT_H

#include <QtCore>
#include <QtNetwork>
#include "common.h"
#include "audiostreaminglibcore.h"
#include "audiostreaminglib_global.h"
//\cond HIDDEN_SYMBOLS
class AUDIOSTREAMING_LIBRARY_EXPORT AbstractClient : public QObject
{
    Q_OBJECT
public:
    explicit AbstractClient(QObject *parent = nullptr);

signals:
    void connected(PeerData,QString);
    void connectedToServer(QByteArray);
    void connectedToPeer(QString);
    void disconnectedFromPeer();
    void webClientLoggedIn();
    void webClientWarning(QString);
    void disconnected(PeerData);
    void readyRead(PeerData);
    void pending(QHostAddress, QString);
    void commandXML(QByteArray);
    void error(QString);

public slots:
    virtual void abort() = 0;
    virtual void connectToHost(const QString &host, quint16 port,
                               const QByteArray &negotiation_string,
                               const QString &id,
                               const QByteArray &password) = 0;
    virtual void writeCommandXML(const QByteArray &XML) = 0;
    virtual void connectToPeer(const QString &peer_id) = 0;
    virtual void disconnectFromPeer() = 0;
    virtual void stop() = 0;
    virtual int write(const QByteArray &data) = 0;
    virtual void acceptSslCertificate() = 0;
    virtual void acceptConnection() = 0;
    virtual void rejectConnection() = 0;
};
//\endcond

#endif // ABSTRACTCLIENT_H
