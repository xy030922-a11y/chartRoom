#pragma once

#include <QObject>
#include <QJsonObject>

#include "ClientSocket.h"





class MessageHandler : public QObject 
{
    Q_OBJECT
    
    public:
        explicit MessageHandler(ClientSocket *clientSocket, QObject *parent = nullptr);

        void sendChatMessage(const QString &text, const QString  &name);

    public slots:
        void onPacketReceived(const QJsonObject &obj);

    signals:
        void chatMessageReceived(const QString &sender, const QString &text, const QString &time);
        void systemMessage(const QString &message);
        void handlerError(const QString &error);
        
    private:
        ClientSocket *clientSocket_;

};