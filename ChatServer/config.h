#ifndef CONFIG_H
#define CONFIG_H

#include <QMessageBox>

class Config
{

public:
    explicit Config();

private:
    QColor m_senderBgColor;
    QColor m_senderTextColor;
    QColor m_receiverBgColor;
    QColor m_receiverTextColor;
    quint32 m_messageBubbleMaxCount;
    quint16 m_fontSize;

    QString m_ip;
    QString m_port;

public:
    void setSenderBgColor(const QColor &color);
    void setSenderTextColor(const QColor &color);
    void setReceiverBgColor(const QColor &color);
    void setReceiverTextColor(const QColor &color);
    void setMessageBubbleMaxCount(const quint32 &messageBubbleMaxCount);
    void setFontSize(const quint16 &fontSize);
    void setIp(const QString &ip);
    void setPort(const QString &port);

    QColor getSenderBgColor() const { return m_senderBgColor; }
    QColor getSenderTextColor() const { return m_senderTextColor; }
    QColor getReceiverBgColor() const { return m_receiverBgColor; }
    QColor getReceiverTextColor() const { return m_receiverTextColor; }

    quint32 getMessageBubbleMaxCount() const { return m_messageBubbleMaxCount; }
    quint16 getFontSize() const { return m_fontSize; }
    QString getIp() const { return m_ip; }
    QString getPort() const { return m_port; }


    void loadConfig();
    void saveConfig();
};



#endif // CONFIG_H
