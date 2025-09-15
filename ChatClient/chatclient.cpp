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
    , m_connectTimeoutMs (CONNECT_TIMEOUT_MS)
    , m_reconnectCount (0)
    , m_reconnectMaxCount(RECONNECT_MAX_COUNT)
{
    ui->setupUi(this);

    setWindowTitle("Chat Client");
    setWindowIcon(QIcon(":/ICON/ICON.png"));

    // 创建控件
    ipLineEdit = new QLineEdit(this);
    ipLineEdit->setPlaceholderText("IP");

    portLineEdit = new QLineEdit(this);
    portLineEdit->setPlaceholderText("端口");

    connectButton = new QPushButton("连接", this);
    connectButton->setCheckable(true);

    sendButton = new QPushButton("发送", this);
    sendButton->setEnabled(false);
    sendButton->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Expanding);

    stateLabel = new QLabel("未连接服务器", this);

    chatWindow = new ChatWindow(this);
    configWidget = new ConfigWidget(this);
    ipLineEdit->setText(configWidget->getIp());
    portLineEdit->setText(configWidget->getPort());
    chatWindow->onConfigMessageBubble(configWidget->getSenderBgColor(),
                                      configWidget->getSenderTextColor(),
                                      configWidget->getReceiverBgColor(),
                                      configWidget->getReceiverTextColor(),
                                      configWidget->getMessageBubbleMaxCount(),
                                      configWidget->getFontSize());


    cleanButton = new QPushButton("清空消息",this);

    inputEdit = new QTextEdit(this);
    inputEdit->setPlaceholderText("输入发送信息");


    timeoutTimer = new QTimer(this);
    timeoutTimer->setSingleShot(true);



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
    splitter->addWidget(chatWindow);
    splitter->addWidget(sendWidget);
    splitter->setStretchFactor(0, 3);  // index 0 = chatWindow
    splitter->setStretchFactor(1, 1);  // index 1 = sendWidget

    connectLayout->addWidget(ipLineEdit);
    connectLayout->addWidget(portLineEdit);
    connectLayout->addWidget(connectButton);
    connectLayout->addWidget(cleanButton);
    connectLayout->addWidget(configWidget);

    sendLayout->addWidget(inputEdit);
    sendLayout->addWidget(sendButton);


    setLayout(mainLayout);

    // 信号槽
    connect(connectButton, &QPushButton::toggled, this, &ChatClient::onClickedConnectButton);
    connect(sendButton, &QPushButton::clicked, this, &ChatClient::onSend);

    connect(socket, &QTcpSocket::connected, this, &ChatClient::onConnected);
    connect(socket, &QTcpSocket::disconnected, this, &ChatClient::onDisconnected);
    connect(socket, &QTcpSocket::readyRead, this, &ChatClient::onReceived);

#if QT_VERSION >= QT_VERSION_CHECK(5, 15, 0)
    connect(socket, &QTcpSocket::errorOccurred, this, &ChatClient::onError);
#else
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(onError(QAbstractSocket::SocketError)));
#endif

    connect(configWidget, &ConfigWidget::configMessageBubble, chatWindow, &ChatWindow::onConfigMessageBubble);
    connect(cleanButton, &QPushButton::clicked, chatWindow, &ChatWindow::onClean);
    connect(timeoutTimer, &QTimer::timeout, this, &ChatClient::onTimeoutTimer);

    this->setStyleSheet(StyleSheet);
}


ChatClient::~ChatClient()
{
    timeoutTimer->stop();
    socket->abort();
    delete ui;
}


void ChatClient::connectHost()
{
    if (m_reconnectCount > m_reconnectMaxCount)
    {
        stateLabel->setText(QString("%1次重新连接失败").arg(m_reconnectCount));
        socket->abort();

        sendButton->setEnabled(false);
        ipLineEdit->setReadOnly(false);
        portLineEdit->setReadOnly(false);
        connectButton->setChecked(false);
        m_reconnectCount=0;

        return;
    }
    ipLineEdit->setReadOnly(true);
    portLineEdit->setReadOnly(true);
    QString ip = ipLineEdit->text();
    quint16 port = portLineEdit->text().toUShort();
    if (ip.isEmpty())
    {
        QMessageBox::warning(this, "错误", "请输入服务器 IP");
        connectButton->setChecked(false);
        return;
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

    if (m_reconnectCount>0)
    {
        stateLabel->setText(QString("第%1次重新连接...  (失败原因 %2)").arg(m_reconnectCount).arg(socket->errorString()));
    }
    else
    {
        stateLabel->setText("正在连接...");
    }
    m_reconnectCount++;
    timeoutTimer->start(m_connectTimeoutMs);

    socket->abort();
    socket->connectToHost(ip, port);
}
void ChatClient::onTimeoutTimer()
{
    if (connectButton->isChecked())
    {
        connectHost();
    }
}

void ChatClient::onError(QAbstractSocket::SocketError socketError)
{
    //qDebug() << "Socket Error:" << socketError << socket->errorString();
    timeoutTimer->stop();
    stateLabel->setText("连接失败: " + socket->errorString());
    socket->abort();

    sendButton->setEnabled(false);
    ipLineEdit->setReadOnly(false);
    portLineEdit->setReadOnly(false);

    if ((socketError == QAbstractSocket::RemoteHostClosedError ||
         socketError == QAbstractSocket::ConnectionRefusedError||
          socketError == QAbstractSocket::SocketTimeoutError ||
          socketError == QAbstractSocket::NetworkError ||
          socketError == QAbstractSocket::ProxyConnectionClosedError||
         socketError == QAbstractSocket::TemporaryError))
    {
        if (connectButton->isChecked())
        {
            connectHost();
        }
    }
    else
    {
        m_reconnectCount = 0;
        connectButton->setChecked(false);
    }
}

void ChatClient::onClickedConnectButton(bool checked)
{
    timeoutTimer->stop();
    m_reconnectCount = 0;
    if (checked)
    {
        connectHost();
    }
    else
    {
        socket->abort();
        stateLabel->setText("已断开连接");
        sendButton->setEnabled(false);
        ipLineEdit->setReadOnly(false);
        portLineEdit->setReadOnly(false);
    }
}

void ChatClient::onConnected()
{
    m_reconnectCount = 0;
    timeoutTimer->stop();
    stateLabel->setText("已连接服务器: " + socket->peerAddress().toString());
    sendButton->setEnabled(true);
}

void ChatClient::onDisconnected()
{
    timeoutTimer->stop();
    stateLabel->setText("未连接服务器");
    sendButton->setEnabled(false);
    if (connectButton->isChecked())
    {
        connectHost();
    }
}



void ChatClient::onReceived()
{
    if (!socket)
    {
        return;
    }
    QByteArray data = socket->readAll();
    chatWindow->addMessage(QString::fromUtf8(data), ChatWindow::Receiver);
}

void ChatClient::onSend()
{
    if (!socket) {
        QMessageBox::warning(this, "错误", "没有客户端连接");
        return;
    }
    QString msg = inputEdit->toPlainText();

    if (msg.isEmpty())
    {
        return;  // 不发送空消息
    }
    socket->write(msg.toUtf8());

    chatWindow->addMessage(msg, ChatWindow::Sender);
}

