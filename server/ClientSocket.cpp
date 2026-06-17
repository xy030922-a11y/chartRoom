#include "ClientSocket.h"


#include <QJsonDocument>
#include <QJsonParseError>
#include <QtEndian>


//-------------public---------------------
ClientSocket::ClientSocket(QObject *parent)
    :QObject(parent),
    socket_(new QTcpSocket(this)) 
{
    connect(socket_, &QTcpSocket::connected,
            this, &ClientSocket::connected);

    connect(socket_, &QTcpSocket::disconnected,
            this, &ClientSocket::disconnected);

    connect(socket_, &QTcpSocket::readyRead,
            this, &ClientSocket::onReadyRead);

    connect(socket_, &QTcpSocket::errorOccurred,
            this, &ClientSocket::onErrorOccurred);
}


void ClientSocket::connectToServer(const QString &host, quint16 port)
{
    socket_->connectToHost(host, port);
}

void ClientSocket::disconnectFromServer()
{
    socket_->disconnectFromHost();
}

void ClientSocket::sendJson(const QJsonObject &obj)
{
    QJsonDocument doc(obj);
    QByteArray body = doc.toJson(QJsonDocument::Compact);

    quint32 len = static_cast<quint32>(body.size());

    QByteArray packet;
    packet.resize(4);

    qToBigEndian<quint32>(
        len,
        reinterpret_cast<uchar *>(packet.data())
    );
    
    packet.append(body);

    socket_->write(packet);
}
        
//-------------private slot---------------------
void ClientSocket::onReadyRead()
{
    readBuffer_.append(socket_->readAll());

    while (true) {
        if (readBuffer_.size() < 4) {
            return;
        }

        quint32 len = qFromBigEndian<quint32>(
            reinterpret_cast<const uchar *>(readBuffer_.constData())
        );

        if (readBuffer_.size() < static_cast<int>(4 + len)) {
            return;
        }

        QByteArray body = readBuffer_.mid(4, len);
        readBuffer_.remove(0, 4 + len);

        QJsonParseError err;
        QJsonDocument doc = QJsonDocument::fromJson(body, &err);

        if (err.error != QJsonParseError::NoError || !doc.isObject()) {
            emit socketError("Invalid JSON packet");
            continue;
        }

        emit packetReceived(doc.object());
    }

}
void ClientSocket::onErrorOccurred(QAbstractSocket::SocketError error)
{
    Q_UNUSED(error);
    emit socketError(socket_->errorString());
}
