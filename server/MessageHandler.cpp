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

void MessageHandler::sendChatMessage(const QString &text, const QString  &name)
{
    QJsonObject obj;
    obj["cmd"] = "chat";
    obj["name"] = name;
    obj["text"] = text;
    obj["time"] = QDateTime::currentDateTime().toString(Qt::ISODate);

    clientSocket_->sendJson(obj);
}

//----------public slots-----------------
void MessageHandler::onPacketReceived(const QJsonObject &obj)
{
    QString cmd = obj["cmd"].toString();


    if (cmd == "chat" || cmd == "chat_message") {
        QString sender = obj["name"].toString();
        QString text = obj["text"].toString();
        QString time = obj["time"].toString();

        emit chatMessageReceived(sender, text, time);
    }
    else if (cmd == "system" ) {
        QString message = obj["message"].toString();

        emit systemMessage(message);
    }
    else if (cmd == "chat_ack") {
        QString ok = obj["ok"].toString();
        QString name = obj["name"].toString();
        QString time = obj["time"].toString();

        QString message = ok + name + time;

        emit systemMessage(message);

    }
    else {
        emit handlerError("Unknown cmd: " + cmd);
    }

}
