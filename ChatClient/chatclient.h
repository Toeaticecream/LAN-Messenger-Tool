#ifndef CHATCLIENT_H
#define CHATCLIENT_H

#include <QWidget>
#include <QTcpSocket>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QTextEdit>

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
    QTextEdit* messageEdit;
    QTextEdit* inputEdit;
    void appendMessage(const QString &prefix, const QString &msg, const QColor &color);
private slots:
    void onConnected();
    void onDisconnected();
    void onReceived();
    void onSend();
    void onClickedConnectButton(bool checked);
};

#endif // CHATCLIENT_H
