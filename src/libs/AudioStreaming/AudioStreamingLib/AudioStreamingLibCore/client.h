#ifndef CLIENT_H
#define CLIENT_H

#include "abstractclient.h"

//\cond HIDDEN_SYMBOLS
class AUDIOSTREAMING_LIBRARY_EXPORT Client : public AbstractClient
{
    Q_OBJECT
public:
    explicit Client(QObject *parent = nullptr);
    ~Client();

public slots:
    void abort();
    void connectToHost(const QString &host, quint16 port,
                       const QByteArray &negotiation_string,
                       const QString &id,
                       const QByteArray &password);
    void writeCommandXML(const QByteArray &XML);
    void connectToPeer(const QString &peer_id);
    void disconnectFromPeer();
    void stop();
    int write(const QByteArray &data);
    void acceptSslCertificate();
    void acceptConnection();
    void rejectConnection();

private slots:
    void timeout();
    void readID();
    void connectedPrivate();
    void disconnectedPrivate();
    void errorPrivate(QAbstractSocket::SocketError e);
    void readyReadPrivate();

private:
    QPointer<QTcpSocket> m_socket;
    QByteArray m_buffer;
    qint32 m_size;

    QPointer<QTimer> m_timer;
    QByteArray m_negotiation_string;
    QString m_remote_id;
    QString m_id;
};
//\endcond

#endif // CLIENT_H
