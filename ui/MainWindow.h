#pragma once

#include <QMainWindow>
#include <QString>
#include <QTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QString>


#include "ClientSocket.h"
#include "MessageHandler.h"


class MainWindow : public QMainWindow 
{
    Q_OBJECT
    public:
        explicit MainWindow(QWidget *parent = nullptr);
        ~MainWindow();

         const QString getName();
    private slots:
        void onConnectButtonClicked();
        void onDisconnectButtonClicked();
        void onSendButtonClicked();

        void onConnected();
        void onDisconnected();
        void onSocketError(const QString &error);

        void onChatMessageReceived(const QString &sender,
                                const QString &text,
                                const QString &time);

        void onSystemMessage(const QString &message);

        void onLoginResult(bool success, const QString &message);

    private:
        void initUi();
        void initConnections();
        void updateConnectedState(bool connected);


    private:
        QTextEdit *chatTextEdit_;
        QLineEdit *messageLineEdit_;
        QPushButton *sendButton_;

        QPushButton *connectButton_;
        QPushButton *disconnectButton;

        QLabel *statusLabel_;

        QLineEdit *hostlineedit;
        QLineEdit *portlineedit;
        QLineEdit *namelineedit_;
            

        ClientSocket *clientSocket_;
        MessageHandler *messageHandler_;


};