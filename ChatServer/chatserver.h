#ifndef CHATSERVER_H
#define CHATSERVER_H

#include <QWidget>
#include <QTcpServer>
#include <QTcpSocket>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QTextEdit>
#include "chatwindow.h"
#include "configwidget.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class ChatServer;
}
QT_END_NAMESPACE

class ChatServer : public QWidget
{
    Q_OBJECT

public:
    ChatServer(QWidget *parent = nullptr);
    ~ChatServer();

private:
    Ui::ChatServer *ui;

private:
    QTcpServer *server;
    QTcpSocket *clientSocket;

    QLineEdit* ipLineEdit;
    QLineEdit* portLineEdit;
    QPushButton* listenButton;
    QPushButton* sendButton;
    QLabel* stateLabel;
    ChatWindow* chatWindow;
    QTextEdit* inputEdit;
    ConfigWidget* configWidget;
    QPushButton* cleanButton;


private slots:
    void onReceived();
    void onSend();
    void onClickedListenButton(bool checked);
    void onConnected();
    void onDisconnected();

};
#endif // CHATSERVER_H
