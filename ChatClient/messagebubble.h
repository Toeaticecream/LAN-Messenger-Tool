#ifndef MESSAGEBUBBLE_H
#define MESSAGEBUBBLE_H

#include <QLabel>
#include <QColor>
#include <QScrollArea>

class MessageBubble : public QScrollArea
{
    Q_OBJECT
public:
    enum BubbleType {
        Sender,   // 自己发送的消息
        Receiver  // 接收的消息
    };

    explicit MessageBubble(const QString &text,
                           BubbleType type = Sender,
                           QWidget *parent = nullptr);

    void setBubbleText(const QString &text);
    void setType(BubbleType type);
    void updateStyle();

private:
    QLabel *messageLabel;
    QWidget *containerWidget;
private:

    BubbleType m_type;

    // 静态颜色方案
    static QColor m_senderBgColor;
    static QColor m_senderTextColor;
    static QColor m_receiverBgColor;
    static QColor m_receiverTextColor;
    static quint16 m_fontSize;

public:
    static void configMessageBubble(const QColor &senderBgColor, const QColor &senderTextColor, const QColor &receiverBgColor, const QColor &receiverTextColor, const quint16 &fontSize);
};

#endif // MESSAGEBUBBLE_H
