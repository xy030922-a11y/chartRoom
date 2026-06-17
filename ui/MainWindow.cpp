#include "MainWindow.h"



#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>



//-------------------public--------------------------
MainWindow::MainWindow(QWidget *parent)
    :QMainWindow(parent),
    chatTextEdit_(nullptr),
    messageLineEdit_(nullptr),
    sendButton_(nullptr),
    connectButton_(nullptr),
    disconnectButton(nullptr),
    statusLabel_(nullptr),
    hostlineedit(nullptr),
    portlineedit(nullptr),
    namelineedit_(nullptr),
    clientSocket_(new ClientSocket(this)),
    messageHandler_(new MessageHandler(clientSocket_, this)) 
{
    initUi();
    initConnections();

    updateConnectedState(false);
}
MainWindow::~MainWindow() = default;

const QString MainWindow::getName()
{
    return namelineedit_->text().trimmed();
}
//-------------private-----------------
void MainWindow::initUi()
{
    auto *central = new QWidget(this);

    chatTextEdit_ = new QTextEdit(central);
    messageLineEdit_ = new QLineEdit(central);
    sendButton_ = new QPushButton("发送", central);

    connectButton_ = new QPushButton("连接", central);
    disconnectButton = new QPushButton("断开", central);

    statusLabel_ = new QLabel("未连接", central);

    hostlineedit = new QLineEdit(central);
    portlineedit = new QLineEdit(central);
    namelineedit_ = new QLineEdit(central);

    hostlineedit->setText("127.0.0.1");
    portlineedit->setText("11111");
    namelineedit_->setPlaceholderText("请输入昵称");

    messageLineEdit_->setPlaceholderText("请输入消息");

    chatTextEdit_->setReadOnly(true);

    auto *mainLayout = new QVBoxLayout(central);

    auto *serverLayout = new QHBoxLayout();

    serverLayout->addWidget(new QLabel("服务器:", central));
    serverLayout->addWidget(hostlineedit);

    serverLayout->addWidget(new QLabel("端口:", central));
    serverLayout->addWidget(portlineedit);

    serverLayout->addWidget(new QLabel("昵称:", central));
    serverLayout->addWidget(namelineedit_);

    serverLayout->addWidget(new QLabel("状态:", central));
    serverLayout->addWidget(statusLabel_);

    serverLayout->addWidget(connectButton_);
    serverLayout->addWidget(disconnectButton);
    
    auto *inputLayout = new QVBoxLayout();
    inputLayout->addWidget(messageLineEdit_);
    inputLayout->addWidget(sendButton_);

    mainLayout->addLayout(serverLayout);
    mainLayout->addWidget(chatTextEdit_);
    mainLayout->addLayout(inputLayout);
    mainLayout->addWidget(statusLabel_);

    central->setLayout(mainLayout);
    setCentralWidget(central);

    setWindowTitle("聊天室客户端");
    resize(700, 500);

}

void MainWindow::initConnections()
{
    connect(connectButton_, &QPushButton::clicked,
            this, &MainWindow::onConnectButtonClicked);

    connect(disconnectButton, &QPushButton::clicked,
            this, &MainWindow::onDisconnectButtonClicked);

    connect(sendButton_, &QPushButton::clicked,
            this, &MainWindow::onSendButtonClicked);

    connect(messageLineEdit_, &QLineEdit::returnPressed,
            this, &MainWindow::onSendButtonClicked);

    connect(clientSocket_, &ClientSocket::connected,
            this, &MainWindow::onConnected);

    connect(clientSocket_, &ClientSocket::disconnected,
            this, &MainWindow::onDisconnected);

    connect(clientSocket_, &ClientSocket::socketError,
            this, &MainWindow::onSocketError);

    connect(messageHandler_, &MessageHandler::chatMessageReceived,
            this, &MainWindow::onChatMessageReceived);

    connect(messageHandler_, &MessageHandler::systemMessage,
            this, &MainWindow::onSystemMessage);
    connect(messageHandler_, &MessageHandler::handlerError,
            this, &MainWindow::onSystemMessage);

}

void MainWindow::updateConnectedState(bool connected)
{
    connectButton_->setEnabled(!connected);
    disconnectButton->setEnabled(connected);
    sendButton_->setEnabled(connected);
    messageLineEdit_->setEnabled(connected);

    hostlineedit->setEnabled(!connected);
    portlineedit->setEnabled(!connected);
    namelineedit_->setEnabled(!connected);
}

//----------------private slots------------------------
void MainWindow::onConnectButtonClicked()
{
    QString host = hostlineedit->text().trimmed();

    bool ok = false;
    quint16 port = portlineedit->text().toUShort(&ok);

    if (host.isEmpty()) {
        chatTextEdit_->append("[系统] 服务器地址不能为空");
        return;
    }

    if (!ok || port == 0) {
        chatTextEdit_->append("[系统] 端口号不合法");
        return;
    }

    QString name = namelineedit_->text().trimmed();
    if (name.isEmpty()) {
        chatTextEdit_->append("[系统] 昵称不能为空");
        return;
    }

    statusLabel_->setText("连接中...");
    clientSocket_->connectToServer(host, port);
}

void MainWindow::onDisconnectButtonClicked()
{
    clientSocket_->disconnectFromServer();
}

void MainWindow::onSendButtonClicked()
{
    QString text = messageLineEdit_->text().trimmed();

    if (text.isEmpty()) {
        return;
    }

    QString name = namelineedit_->text().trimmed();
    if (name.isEmpty()) {
        name = "我";
    }

    messageHandler_->sendChatMessage(text, name);

    chatTextEdit_->append(name + ": " + text);
    messageLineEdit_->clear();
}

void MainWindow::onConnected()
{
    chatTextEdit_->append("已连接服务器");
    statusLabel_->setText("已连接");

    updateConnectedState(true); 
}

void MainWindow::onDisconnected()
{
    chatTextEdit_->append("已断开连接");
    statusLabel_->setText("未连接");

    updateConnectedState(false);
}

void MainWindow::onSocketError(const QString &error)
{
    chatTextEdit_->append("网络错误: " + error);
    statusLabel_->setText("连接错误");

    updateConnectedState(false);
}

void MainWindow::onChatMessageReceived(const QString &sender,
                                const QString &text,
                                const QString &time)
{
    Q_UNUSED(time);

    if(sender == getName())
    {
        return;
    }
    chatTextEdit_->append(sender + ": " + text);
}

void MainWindow::onSystemMessage(const QString &message)
{
    chatTextEdit_->append("[系统] " + message);
}

void MainWindow::onLoginResult(bool success, const QString &message)
{
    chatTextEdit_->append(QString("[登录%1] %2")
                              .arg(success ? "成功" : "失败", message));
}
