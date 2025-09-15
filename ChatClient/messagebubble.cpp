#include "messagebubble.h"
#if QT_VERSION < QT_VERSION_CHECK(5, 15, 0)
    #include <QVariant>
#endif



#include <QVBoxLayout>
// 定义静态常量
QColor MessageBubble::m_senderBgColor = QColor(0, 180, 255);   // 蓝色
QColor MessageBubble::m_senderTextColor = Qt::white;
QColor MessageBubble::m_receiverBgColor = QColor(240, 240, 240); // 灰色
QColor MessageBubble::m_receiverTextColor = Qt::black;
quint16 MessageBubble::m_fontSize = 14;

MessageBubble::MessageBubble(const QString &text, BubbleType type, QWidget *parent)
    : QScrollArea(parent), m_type(type)
{
#if QT_VERSION >= QT_VERSION_CHECK(5, 15, 0)
    setProperty("MessageBubble",true);
#else
    setProperty("MessageBubble",QVariant(true));
#endif


    setWidgetResizable(true);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    containerWidget = new QWidget(this);
#if QT_VERSION >= QT_VERSION_CHECK(5, 15, 0)
    containerWidget->setProperty("ContainerWidget",true);
#else
    containerWidget->setProperty("ContainerWidget",QVariant(true));
#endif

    containerWidget->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    QHBoxLayout *containerLayout = new QHBoxLayout;
    QVBoxLayout *vLayout = new QVBoxLayout(containerWidget);
    vLayout->setAlignment(Qt::AlignTop);

    messageLabel = new QLabel(this);
    setBubbleText(text);
    updateStyle();
    messageLabel->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    messageLabel->setTextInteractionFlags(Qt::TextSelectableByMouse); // 可复制

    setWidget(containerWidget);
    vLayout->addLayout(containerLayout);

    if(type == Sender)
    {
        containerLayout->addStretch();
        containerLayout->addWidget(messageLabel);
    }
    else
    {
        containerLayout->addWidget(messageLabel);
        containerLayout->addStretch();
    }
}

void MessageBubble::setBubbleText(const QString &text)
{
    messageLabel->setStyleSheet(QString("font-size: %1pt;").arg(m_fontSize));
    messageLabel->setText(text);
}

void MessageBubble::setType(BubbleType type)
{
    m_type = type;
    updateStyle();
}

void MessageBubble::updateStyle()
{
    if (m_type == Sender)
    {
        messageLabel->setStyleSheet(QString("QLabel { font-size: %1pt; background-color: rgb(%2,%3,%4); color: rgb(%5,%6,%7); border-radius: 10px; padding: 4px; }")
                            .arg(m_fontSize)
                            .arg(m_senderBgColor.red())
                            .arg(m_senderBgColor.green())
                            .arg(m_senderBgColor.blue())
                            .arg(m_senderTextColor.red())
                            .arg(m_senderTextColor.green())
                            .arg(m_senderTextColor.blue()));
    }
    else
    {
        messageLabel->setStyleSheet(QString("QLabel { font-size: %1pt; background-color: rgb(%2,%3,%4); color: rgb(%5,%6,%7); border-radius: 10px; padding: 4px; }")
                            .arg(m_fontSize)
                            .arg(m_receiverBgColor.red())
                            .arg(m_receiverBgColor.green())
                            .arg(m_receiverBgColor.blue())
                            .arg(m_receiverTextColor.red())
                            .arg(m_receiverTextColor.green())
                            .arg(m_receiverTextColor.blue()));
    }
}


void MessageBubble::configMessageBubble(const QColor &senderBgColor, const QColor &senderTextColor, const QColor &receiverBgColor, const QColor &receiverTextColor, const quint16 &fontSize)
{
    m_senderBgColor = senderBgColor;
    m_senderTextColor= senderTextColor;
    m_receiverBgColor = receiverBgColor;
    m_receiverTextColor= receiverTextColor;
    m_fontSize = fontSize;
}
