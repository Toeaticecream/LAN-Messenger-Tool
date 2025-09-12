#include "chatclient.h"
#include "ui_chatclient.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QHostAddress>
#include <QSplitter>
#include "resource.h"
ChatClient::ChatClient(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ChatClient)
    , socket(new QTcpSocket(this))
{
    ui->setupUi(this);

    setWindowTitle("Chat Client");
    setWindowIcon(QIcon(":/ICON/ICON.png"));

    // 创建控件
    ipLineEdit = new QLineEdit(this);
    ipLineEdit->setPlaceholderText("IP");
    ipLineEdit->setText("192.168.137.1");

    portLineEdit = new QLineEdit(this);
    portLineEdit->setPlaceholderText("端口");
    portLineEdit->setText("8080");

    connectButton = new QPushButton("连接", this);
    connectButton->setCheckable(true);

    sendButton = new QPushButton("发送", this);
    sendButton->setEnabled(false);
    sendButton->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Expanding);

    stateLabel = new QLabel("未连接服务器", this);

    messageEdit = new QTextEdit(this);
    messageEdit->setReadOnly(true);
    messageEdit->setWordWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
    messageEdit->setPlaceholderText("历史消息:");


    inputEdit = new QTextEdit(this);
    inputEdit->setPlaceholderText("输入发送信息");
    // 布局
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    QHBoxLayout* connectLayout = new QHBoxLayout();
    QSplitter* splitter = new QSplitter(this);
    splitter->setOrientation(Qt::Vertical);
    QWidget* sendWidget = new QWidget(this);
    QHBoxLayout* sendLayout = new QHBoxLayout(sendWidget);

    mainLayout->addLayout(connectLayout);
    mainLayout->addWidget(stateLabel);
    mainLayout->addWidget(splitter);
    splitter->addWidget(messageEdit);
    splitter->addWidget(sendWidget);


    connectLayout->addWidget(ipLineEdit);
    connectLayout->addWidget(portLineEdit);
    connectLayout->addWidget(connectButton);


    sendLayout->addWidget(inputEdit);
    sendLayout->addWidget(sendButton);


    setLayout(mainLayout);

    // 信号槽
    connect(connectButton, &QPushButton::toggled, this, &ChatClient::onClickedConnectButton);
    connect(sendButton, &QPushButton::clicked, this, &ChatClient::onSend);

    connect(socket, &QTcpSocket::connected, this, &ChatClient::onConnected);
    connect(socket, &QTcpSocket::disconnected, this, &ChatClient::onDisconnected);
    connect(socket, &QTcpSocket::readyRead, this, &ChatClient::onReceived);
    this->setStyleSheet(StyleSheet);
}

ChatClient::~ChatClient()
{
    socket->abort();
    delete ui;
}

void ChatClient::onClickedConnectButton(bool checked)
{
    if (checked)
    {
        ipLineEdit->setReadOnly(true);
        portLineEdit->setReadOnly(true);
        QString ip = ipLineEdit->text();
        quint16 port = portLineEdit->text().toUShort();

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
               connectButton->setChecked(false);
               return;
           }
        }
        socket->connectToHost(ip, port);
        stateLabel->setText("正在连接...");
    }
    else
    {
        socket->disconnectFromHost();
        stateLabel->setText("已断开连接");
        sendButton->setEnabled(false);

        ipLineEdit->setReadOnly(false);
        portLineEdit->setReadOnly(false);
    }
}

void ChatClient::onConnected()
{
    stateLabel->setText("已连接服务器: " + socket->peerAddress().toString());
    sendButton->setEnabled(true);
}

void ChatClient::onDisconnected()
{
    stateLabel->setText("未连接服务器");
    sendButton->setEnabled(false);
    if (connectButton->isChecked())
    {
        QString ip = ipLineEdit->text();
        quint16 port = portLineEdit->text().toUShort();
        if (ip.isEmpty())
        {
            QMessageBox::warning(this, "错误", "请输入服务器 IP");
            connectButton->setChecked(false);
            return;
        }
        socket->connectToHost(ip, port);
        stateLabel->setText("正在连接...");
    }
}

void ChatClient::onReceived()
{
    QByteArray data = socket->readAll();
    appendMessage("服务端", QString::fromUtf8(data), Qt::white);
}

void ChatClient::onSend()
{
    if (!socket->isOpen())
    {
        QMessageBox::warning(this, "错误", "未连接服务器");
        return;
    }
    QString msg = inputEdit->toPlainText();
    socket->write(msg.toUtf8());

    appendMessage("客户端",msg,Qt::yellow);
}

void ChatClient::appendMessage(const QString &prefix, const QString &msg, const QColor &color)
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

