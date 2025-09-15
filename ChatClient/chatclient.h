#ifndef CHATCLIENT_H
#define CHATCLIENT_H

#include <QWidget>
#include <QTcpSocket>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QTextEdit>
#include <QTimer>
#include "chatwindow.h"
#include "configwidget.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class ChatClient;
}
QT_END_NAMESPACE

class ChatClient : public QWidget
{
    Q_OBJECT

public:
    ChatClient(QWidget *parent = nullptr);
    ~ChatClient();

private:
    Ui::ChatClient *ui;

private:
    QTcpSocket *socket;

    QLineEdit* ipLineEdit;
    QLineEdit* portLineEdit;
    QPushButton* connectButton;
    QPushButton* sendButton;
    QLabel* stateLabel;
    ChatWindow* chatWindow;
    QTextEdit* inputEdit;
    ConfigWidget* configWidget;
    QPushButton* cleanButton;

    QTimer *timeoutTimer;
    int m_connectTimeoutMs = 5000;  // 超时时间，单位毫秒
    int m_reconnectCount = 0;
    int m_reconnectMaxCount = 0;

private:
    void connectHost();
private slots:
    void onConnected();
    void onDisconnected();
    void onReceived();
    void onSend();
    void onClickedConnectButton(bool checked);
    void onError(QAbstractSocket::SocketError socketError);
    void onTimeoutTimer();
};

#endif // CHATCLIENT_H
