#ifndef CHATWINDOW_H
#define CHATWINDOW_H

#include <QWidget>
#include <QScrollArea>
#include <QVBoxLayout>
#include "messagebubble.h"

class ChatWindow : public QWidget
{
    Q_OBJECT
public:
    explicit ChatWindow(QWidget *parent = nullptr);

    enum MessageType {
        Sender,
        Receiver
    };

    void addMessage(const QString &text, MessageType type);

private:    
    QScrollArea *scrollArea;
    QWidget *containerWidget;
    QVBoxLayout *vLayout;
    QVector<MessageBubble*> messageBubbleVector;
    qint32 m_messageBubbleMaxCount;
protected:
    void resizeEvent(QResizeEvent *event) override;
private:
    void removeMessage();
public slots:
    void onConfigMessageBubble(const QColor &senderBgColor, const QColor &senderTextColor, const QColor &receiverBgColor, const QColor &receiverTextColor, const qint32 &messageBubbleMaxCount, const quint16 &fontSize);
    void onClean();
};

#endif // CHATWINDOW_H
