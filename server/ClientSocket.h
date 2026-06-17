#pragma once

#include <QObject>
#include <QTcpSocket>
#include <QByteArray>
#include <QJsonObject>




class ClientSocket : public QObject {
    Q_OBJECT

    public:
        explicit ClientSocket(QObject *parent = nullptr);

        void connectToServer(const QString &host, quint16 port);
        void disconnectFromServer();

        void sendJson(const QJsonObject &obj);
        
    signals:
        void connected();
        void disconnected();
        void packetReceived(const QJsonObject &obj);
        void socketError(const QString &error);

    private slots:
        void onReadyRead();
        void onErrorOccurred(QAbstractSocket::SocketError error);

    private:
        QTcpSocket *socket_;
        QByteArray readBuffer_;
};