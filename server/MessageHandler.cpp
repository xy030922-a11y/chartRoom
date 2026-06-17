#include "MessageHandler.h"


#include <QDateTime>





//----------public-----------------
MessageHandler::MessageHandler(ClientSocket *clientSocket,
                               QObject *parent)
    :QObject(parent),
    clientSocket_(clientSocket) 
{
    connect(clientSocket_, &ClientSocket::packetReceived,
            this, &MessageHandler::onPacketReceived);
}

void MessageHandler::sendChatMessage(const QString &text)
{
    QJsonObject obj;
    obj["cmd"] = "chat";
    obj["text"] = text;
    obj["time"] = QDateTime::currentDateTime().toString(Qt::ISODate);

    clientSocket_->sendJson(obj);
}

//----------public slots-----------------
void MessageHandler::onPacketReceived(const QJsonObject &obj)
{
    QString cmd = obj["cmd"].toString();


    if (cmd == "chat") {
        QString sender = obj["sender"].toString();
        QString text = obj["text"].toString();
        QString time = obj["time"].toString();

        emit chatMessageReceived(sender, text, time);
    }
    else if (cmd == "system") {
        QString message = obj["message"].toString();

        emit systemMessage(message);
    }
    else {
        emit handlerError("Unknown cmd: " + cmd);
    }

}
