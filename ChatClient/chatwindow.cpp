#include "chatwindow.h"

#include <QHBoxLayout>
#include <QScrollBar>
#include <QTimer>
#if QT_VERSION < QT_VERSION_CHECK(5, 15, 0)
    #include <QVariant>
#endif
ChatWindow::ChatWindow(QWidget *parent) : QWidget(parent)
{
    scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    containerWidget = new QWidget;
#if QT_VERSION >= QT_VERSION_CHECK(5, 15, 0)
    containerWidget->setProperty("ContainerWidget", true);
#else
    containerWidget->setProperty("ContainerWidget", QVariant(true));
#endif

    vLayout = new QVBoxLayout(containerWidget);
    vLayout->setAlignment(Qt::AlignTop);
    vLayout->setSpacing(10);
    containerWidget->setLayout(vLayout);

    scrollArea->setWidget(containerWidget);
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(scrollArea);
    setLayout(mainLayout);
}

void ChatWindow::addMessage(const QString &text, MessageType type)
{
    MessageBubble *bubble = new MessageBubble(text, type == Sender ? MessageBubble::Sender : MessageBubble::Receiver);
    messageBubbleVector.append(bubble);

    bubble->setFixedWidth(scrollArea->width() * 0.7);
    QHBoxLayout *hLayout = new QHBoxLayout;

    hLayout->addWidget(bubble);  // 先把 bubble 放进去
    hLayout->setAlignment(bubble, (type == Sender) ? Qt::AlignRight : Qt::AlignLeft);

    vLayout->addLayout(hLayout);

    // 超出最大数量，删除最早的消息
    removeMessage();

    // 自动滚动到底部
    QTimer::singleShot(100, this, [this] {
        // 滚动到最底部
        scrollArea->verticalScrollBar()->setValue(scrollArea->verticalScrollBar()->maximum());
    });
}

void ChatWindow::removeMessage()
{
    while (messageBubbleVector.size() > m_messageBubbleMaxCount)
    {
        MessageBubble *oldBubble = messageBubbleVector.takeFirst(); // 取出第一个
        // 删除对应的布局
        QLayoutItem *item = vLayout->takeAt(0);
        if (item)
        {
            // 删除布局中的 widget
            QHBoxLayout *hl = dynamic_cast<QHBoxLayout*>(item);
            if (hl)
            {
                QLayoutItem *childItem;
                while ((childItem = hl->takeAt(0)) != nullptr)
                {
                    if (QWidget *w = childItem->widget())
                    {
                        w->deleteLater(); // 删除 widget
                    }
                    delete childItem;
                }
                delete hl;
            }
            else
            {
                delete item;
            }
        }
        if (oldBubble)
        {
            oldBubble->deleteLater(); // 删除消息泡泡
        }
    }
}

void ChatWindow::onConfigMessageBubble(const QColor &senderBgColor, const QColor &senderTextColor, const QColor &receiverBgColor, const QColor &receiverTextColor, const qint32 &messageBubbleMaxCount, const quint16 &fontSize)
{
    m_messageBubbleMaxCount = messageBubbleMaxCount;
    removeMessage();
    MessageBubble::configMessageBubble(senderBgColor,senderTextColor,receiverBgColor,receiverTextColor,fontSize);

    if (!messageBubbleVector.isEmpty())
    {
        for (int i = 0; i < messageBubbleVector.size(); ++i)
        {
            messageBubbleVector[i]->updateStyle();
        }
    }

}

void ChatWindow::onClean()
{
    while (messageBubbleVector.size() > 0)
    {
        MessageBubble *oldBubble = messageBubbleVector.takeFirst(); // 取出第一个
        // 删除对应的布局
        QLayoutItem *item = vLayout->takeAt(0);
        if (item)
        {
            // 删除布局中的 widget
            QHBoxLayout *hl = dynamic_cast<QHBoxLayout*>(item);
            if (hl)
            {
                QLayoutItem *childItem;
                while ((childItem = hl->takeAt(0)) != nullptr)
                {
                    if (QWidget *w = childItem->widget())
                    {
                        w->deleteLater(); // 删除 widget
                    }
                    delete childItem;
                }
                delete hl;
            }
            else
            {
                delete item;
            }
        }
        if (oldBubble)
        {
            oldBubble->deleteLater(); // 删除消息泡泡
        }
    }
}

void ChatWindow::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);

    if (!messageBubbleVector.isEmpty())
    {
        int maxWidth = containerWidget->width() * 0.7; // 70% 宽度
        for (int i = 0; i < messageBubbleVector.size(); ++i)
        {
            messageBubbleVector[i]->setFixedWidth(maxWidth);
        }
    }
}
