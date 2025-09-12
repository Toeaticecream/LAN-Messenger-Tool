#ifndef CHATSERVER_H
#define CHATSERVER_H

#include <QWidget>
#include <QTcpServer>
#include <QTcpSocket>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QTextEdit>


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
    QTextEdit* messageEdit;
    QTextEdit* inputEdit;

    void appendMessage(const QString &prefix, const QString &msg, const QColor &color);

private slots:
    void onReceived();
    void onSend();
    void onClickedListenButton(bool checked);
    void onConnected();
    void onDisconnected();
};
#endif // CHATSERVER_H
