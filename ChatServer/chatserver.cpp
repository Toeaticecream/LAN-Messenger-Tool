#include "chatserver.h"
#include "ui_chatserver.h"


#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHostAddress>
#include <QMessageBox>
#include <QSplitter>
#include "resource.h"
ChatServer::ChatServer(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ChatServer)
    , server(new QTcpServer(this))
    , clientSocket(nullptr)
{
    ui->setupUi(this);

    setWindowIcon(QIcon(":/ICON/ICON.png"));
    // 创建控件
    ipLineEdit = new QLineEdit(this);
    ipLineEdit->setPlaceholderText("IP");
    ipLineEdit->setText("192.168.137.1");

    portLineEdit = new QLineEdit(this);
    portLineEdit->setPlaceholderText("端口");
    portLineEdit->setText("8080");

    listenButton = new QPushButton("监听", this);
    listenButton->setCheckable(true);
    sendButton = new QPushButton("发送", this);
    sendButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);

    stateLabel = new QLabel("未连接客户端", this);
    messageEdit = new QTextEdit(this);
    messageEdit->setReadOnly(true);     // 设置只读
    messageEdit->setWordWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere); // 自动换行
    messageEdit->setPlaceholderText("消息记录:");   // 初始文本

    inputEdit = new QTextEdit(this);
    inputEdit->setPlaceholderText("输入发送信息");
    sendButton->setEnabled(false); // 默认没有客户端连接不可发送

    // 布局
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QHBoxLayout *connectLayout = new QHBoxLayout();
    QSplitter *splitter = new QSplitter(this);
    splitter->setOrientation(Qt::Vertical);
    QWidget* sendWidget = new QWidget(this);
    QHBoxLayout *sendLayout = new QHBoxLayout(sendWidget);

    mainLayout->addLayout(connectLayout);
    mainLayout->addWidget(stateLabel);
    mainLayout->addWidget(splitter);
    splitter->addWidget(messageEdit);
    splitter->addWidget(sendWidget);

    connectLayout->addWidget(ipLineEdit);
    connectLayout->addWidget(portLineEdit);
    connectLayout->addWidget(listenButton);


    sendLayout->addWidget(inputEdit);
    sendLayout->addWidget(sendButton);

    setLayout(mainLayout);

    // 信号槽
    connect(listenButton, &QPushButton::toggled, this, &ChatServer::onClickedListenButton);
    connect(sendButton, &QPushButton::clicked, this, &ChatServer::onSend);
    connect(server, &QTcpServer::newConnection, this, &ChatServer::onConnected);


    this->setStyleSheet(StyleSheet);
}

ChatServer::~ChatServer()
{
    if (clientSocket)
    {
        clientSocket->abort();
    }
    server->close();
    delete ui;
}

void ChatServer::onConnected()
{
    if (clientSocket)
    {
        return;
    }
    clientSocket = server->nextPendingConnection();
    stateLabel->setText("已连接客户端: " + clientSocket->peerAddress().toString());
    sendButton->setEnabled(true);

    connect(clientSocket, &QTcpSocket::readyRead, this, &ChatServer::onReceived);
    connect(clientSocket, &QTcpSocket::disconnected, this, &ChatServer::onDisconnected);
}

void ChatServer::onDisconnected()
{
    disconnect(clientSocket, &QTcpSocket::readyRead, this, &ChatServer::onReceived);
    disconnect(clientSocket, &QTcpSocket::disconnected, this, &ChatServer::onDisconnected);

    stateLabel->setText("未连接客户端");
    sendButton->setEnabled(false);
    if (clientSocket)
    {
        clientSocket->abort();
    }
    clientSocket = nullptr;
}


void ChatServer::onClickedListenButton(bool checked)
{
    if (checked)
    {
        ipLineEdit->setReadOnly(true);
        portLineEdit->setReadOnly(true);

        QString ip = ipLineEdit->text();

        if (ip.isEmpty())
        {
            ip = "0.0.0.0"; // 默认绑定所有地址
        }
        else
        {
            QHostAddress address;
            if (!address.setAddress(ip))
            {
                QMessageBox::warning(this, "错误", "请输入有效的IP地址");
                listenButton->setChecked(false);
                return;
            }
        }
        if (!server->listen(QHostAddress(ip), portLineEdit->text().toUShort()))
        {
            QMessageBox::warning(this, "错误", "启动服务器失败");
            listenButton->setChecked(false);
            return;
        }
        stateLabel->setText("\n服务器已启动，等待客户端连接...");
    }
    else // 停止服务器
    {
        if (clientSocket)
        {
            clientSocket->abort();
        }
        server->close();
        stateLabel->setText("服务器已停止");
        listenButton->setText("监听");
        sendButton->setEnabled(false);
        ipLineEdit->setReadOnly(false); // 可以修改IP
        portLineEdit->setReadOnly(false);
    }
}

void ChatServer::onReceived()
{
    if (!clientSocket) return;
    QByteArray data = clientSocket->readAll();
    appendMessage("客户端",QString::fromUtf8(data),Qt::white);
}

void ChatServer::onSend()
{
    if (!clientSocket) {
        QMessageBox::warning(this, "错误", "没有客户端连接");
        return;
    }
    QString msg = inputEdit->toPlainText();

    if (msg.isEmpty()) {
        return;  // 不发送空消息
    }
    clientSocket->write(msg.toUtf8());

    appendMessage("服务端",msg,Qt::yellow);
}

void ChatServer::appendMessage(const QString &prefix, const QString &msg, const QColor &color)
{
    QTextCursor cursor = messageEdit->textCursor();
    cursor.movePosition(QTextCursor::End);

    QTextCharFormat format_old = cursor.charFormat();
    QTextCharFormat format = format_old;
    format.setForeground(color);

    cursor.insertText(prefix + "\n" + msg + "\n", format);

    cursor.setCharFormat(format_old);

    cursor.insertText("\n", format_old);
}
